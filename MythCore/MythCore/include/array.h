#ifndef __ARRAY_H__
#define __ARRAY_H__
#include "commontype.h"
#include "iterator.h"
#include <xutility>
namespace Myth
{
	template<typename T, uint32 Capacity>
	class CArray
	{
		typedef CArray<T, Capacity>	array_type;
	public:
		typedef		T				value_type;
		typedef 	value_type*		pointer;
		typedef		value_type&		reference;
		typedef		size_t			size_type;
		typedef		ptrdiff_t		difference_type;
		typedef const value_type*	const_pointer;
		typedef const value_type&	const_reference;
		typedef MythInternal::__normal_iterator<pointer, array_type>			iterator;
		typedef MythInternal::__normal_iterator<const_pointer, array_type>	const_iterator;
		typedef MythInternal::reverse_iterator<const_iterator>				const_reverse_iterator;
		typedef MythInternal::reverse_iterator<iterator>						reverse_iterator;

	public:
		CArray()
		{
			mSize = 0;
		}
		CArray(uint32 nSize, T t=T())
		{
			std::fill(begin().base(), (begin() + nSize).base(), t);
			mSize = nSize;
		}

		template <class InputIterator>
		CArray(InputIterator first, InputIterator last)
		{
			uint32 nSize = last - first;
			if (nSize <= 0)
			{
				return;
			}
			iterator cur = begin();
			for (uint32 i = 0; i < nSize; ++ i)
			{
				*cur++ = *first++;
			}
			mSize = nSize;
		}

		CArray(CArray& rArray)
		{
			std::copy(rArray.begin().base(), rArray.end().base(), begin().base());
			mSize = rArray.mSize;
		}
		~CArray(){}

		CArray& operator=(CArray& rArray)
		{
			std::copy(rArray.begin().base(), rArray.end().base(), begin().base());
			mSize = rArray.mSize;
		}

		iterator begin()
		{
			return (iterator)(mData);
		}
		const_iterator begin() const
		{
			return (const_iterator)(mData);
		}
		
		iterator end()
		{
			return (iterator)(mData + mSize);
		}
		
		const_iterator end() const
		{
			return (const_iterator)(mData + mSize);
		}

		size_type size() const
		{
			return (size_type)mSize;
		}

		size_type max_size() const
		{
			return (size_type)Capacity;
		}

		size_type capacity() const
		{
			return (size_type)Capacity;
		}

		bool empty() const
		{
			return mSize == 0;
		}

		reference operator[] (size_type n)
		{
			return mData[n];
		}
		const_reference operator[] (size_type n) const
		{
			return mData[n];
		}

		reference at (size_type n)
		{
			if (n < 0  || n>= mSize)
			{
				// Log
			}

			return mData[n];
		}
		const_reference at (size_type n) const
		{
			if (n < 0  || n>= mSize)
			{
				// Log
			}

			return mData[n];
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
			if (n > Capacity)
			{
				return;
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
			iterator cur = begin();
			for (uint32 i = 0; i < nSize && i < mSize; ++ i)
			{
				*cur++ = *first++;
			}
			mSize = nSize;
		}

		void push_back (const value_type& val)
		{
			if (mSize >= Capacity)
			{
				return;
			}
			mData[mSize] = val;
			++ mSize;
		}

		void pop_back()
		{
			-- mSize;
		}
		iterator insert (iterator position, const value_type& val)
		{
			if (mSize >= Capacity)
			{
				return end();
			}

			std::copy_backward(position.base(), end().base(), (end() + 1).base());
			*position = val;
			++ mSize;
			return position;
		}
		void insert (iterator position, size_type n, const value_type& val)
		{
			if (mSize + n >= Capacity)
			{
				return;
			}
			std::copy_backward(position.base(), end().base(), (begin() + mSize + n).base());
			std::fill(position.base(), (position + n).base(), val);
			mSize += n;
		}
		void insert (iterator position, iterator first, iterator last)
		{
			uint nSize = last - first;
			if (mSize + nSize >= Capacity)
			{
				return ;
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
		T			mData[Capacity];
		uint32		mSize;
	};
}

#endif