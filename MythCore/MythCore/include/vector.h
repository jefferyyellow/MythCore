#ifndef __VECTOR_H__
#define __VECTOR_H__
#include "iterator.h"
#include "commontype.h"
#include <xutility>
namespace Myth
{
	template<typename T, uint BaseCount=32, uint Increment=8>
	class CVector
	{
		typedef CVector<T, BaseCount, Increment>	vector_type;
	public:
		typedef		T				value_type;
		typedef 	value_type*		pointer;
		typedef		value_type&		reference;
		typedef		size_t			size_type;
		typedef		ptrdiff_t		difference_type;
		typedef const value_type*	const_pointer;
		typedef const value_type&	const_reference;
		typedef MythInternal::__normal_iterator<pointer, vector_type>			iterator;
		typedef MythInternal::__normal_iterator<const_pointer, vector_type>		const_iterator;
		typedef MythInternal::reverse_iterator<const_iterator>					const_reverse_iterator;
		typedef MythInternal::reverse_iterator<iterator>						reverse_iterator;

	public:
		CVector()
		{
			mpData = new T[BaseCount];
			mSize = 0;
			mCapacity = BaseCount;
		}
		CVector(uint32 nSize, T t=T())
		{
			mpData = NULL;
			if (nSize > BaseCount)
			{
				Resize(nSize);
			}
			else
			{
				mpData = new T[BaseCount];
				mCapacity = BaseCount;
			}

			std::fill(mpData, mpData + nSize, t);
			mSize = nSize;
		}

		template <class InputIterator>
		CVector(InputIterator first, InputIterator last)
		{
			mpData = NULL;
			uint32 nSize = last - first;
			if (nSize <= 0)
			{
				return;
			}
			if (nSize > BaseCount)
			{
				Resize(nSize);
			}
			else
			{
				mpData = new T[BaseCount];
				mCapacity = BaseCount;
			}
			mpData = new T[mCapacity];
			iterator cur = begin();
			for (uint32 i = 0; i < nSize; ++ i)
			{
				*cur++ = *first++;
			}
			mSize = nSize;
		}

		CVector(CVector& rVector)
		{
			mpData = new T[rVector.mCapacity];
			std::copy(rVector.begin().base(), rVector.end().base(), begin().base());
			mSize = rVector.mSize;
			mCapacity = rVector.mCapacity;
		}
		~CVector(){}

		CVector& operator=(CVector& rVector)
		{
			if (mCapacity < rVector.mSize)
			{
				Resize(rVector.mSize);
			}
			std::copy(rVector.begin().base(), rVector.end().base(), begin().base());
			mSize = rVector.mSize;
		}
		iterator begin()
		{
			return (iterator)(mpData);
		}
		const_iterator begin() const
		{
			return (const_iterator)(mpData);
		}

		iterator end()
		{
			return (iterator)(mpData + mSize);
		}

		const_iterator end() const
		{
			return (const_iterator)(mpData + mSize);
		}

		size_type size() const
		{
			return (size_type)mSize;
		}

		size_type max_size() const
		{
			return (size_type)mCapacity;
		}

		size_type capacity() const
		{
			return (size_type)mCapacity;
		}

		bool empty() const
		{
			return mSize == 0;
		}

		reference operator[] (size_type n)
		{
			return mpData[n];
		}
		const_reference operator[] (size_type n) const
		{
			return mpData[n];
		}

		reference at (size_type n)
		{
			if (n < 0  || n>= mSize)
			{
				return mpData[0];
			}

			return mpData[n];
		}
		const_reference at (size_type n) const
		{
			if (n < 0  || n>= mSize)
			{
				// Log
				return mpData[0];
			}

			return mpData[n];
		}

		reference front()
		{
			return *begin();
		}
		const_reference front() const
		{
			return *begin();
		}

		reference back()
		{
			return *end();
		}

		const_reference back() const
		{
			return *end();
		}

		void assign (int n, const value_type& val)
		{
			assign(size_type(n), val);
		}

		void assign (size_type n, const value_type& val)
		{
			if (n > mCapacity)
			{
				Resize(n);
			}

			std::fill(begin().base(), (begin() + n).base(), val);
			mSize = n;
		}

		template <typename InputIterator>
		void assign (InputIterator first, InputIterator last)
		{
			uint32 nSize = last - first;
			if (nSize <= 0)
			{
				return;
			}

			if (nSize > mCapacity)
			{
				Resize(nSize);
			}

			iterator cur = begin();
			for (uint32 i = 0; i < nSize; ++ i)
			{
				*cur++ = *first++;
			}
			mSize = nSize;
		}

		bool		Resize(int nSize)
		{
			int nMode = (nSize - BaseCount) % Increment;
			mCapacity = BaseCount + (nSize - BaseCount) / Increment * Increment;
			mCapacity += nMode > 0 ? Increment : 0;
			T* pNewData = new T[mCapacity];
			if (NULL != mpData)
			{
				memcpy(pNewData, mpData,  mSize * sizeof(T));
				delete []mpData;
			}
			mpData = pNewData;
			return true;
		}

		void push_back (const value_type& val)
		{
			if (mSize >= mCapacity)
			{
				Resize(mSize + Increment);
			}
			mpData[mSize] = val;
			++ mSize;
		}

		void pop_back()
		{
			-- mSize;
		}
		iterator insert (iterator position, const value_type& val)
		{
			if (mSize >= mCapacity)
			{
				Resize(mSize + Increment);
			}

			std::copy_backward(position.base(), end().base(), (end() + 1).base());
			*position = val;
			++ mSize;
			return position;
		}
		void insert (iterator position, size_type n, const value_type& val)
		{
			if (mSize + n >= mCapacity)
			{
				Resize(mSize + n);
			}
			std::copy_backward(position.base(), end().base(), (begin() + mSize + n).base());
			std::fill(position.base(), (position + n).base(), val);
			mSize += n;
		}
		void insert (iterator position, iterator first, iterator last)
		{
			uint nSize = last - first;
			if (mSize + nSize >= mCapacity)
			{
				Resize(mSize + Increment);
			}
			std::copy_backward(position.base(), end().base(), (begin() + mSize + nSize).base());
			std::copy(first.base(), last.base(), position.base());

			mSize += nSize;
		}

		iterator erase (iterator position)
		{
			if (position + 1 != end())
			{
				std::copy((position + 1).base(), end().base(), position.base());
			}
			-- mSize;
			return position;
		}

		iterator erase (iterator first, iterator last)
		{
			if (last != end())
			{
				std::copy(last.base(), end().base(), first.base());
			}
			mSize -= (last - first);
			return first;
		}

		void clear()
		{
			mSize = 0;
		}
	private:
		T*			mpData;
		uint32		mSize;
		uint32		mCapacity;
	};
}
#endif