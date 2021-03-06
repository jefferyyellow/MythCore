#ifndef __ARRAY_H__
#define __ARRAY_H__
#include "commontype.h"
#include <algorithm>
namespace Myth
{
	template<typename T, int Capacity>
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
		typedef pointer				iterator;
		typedef const_pointer		const_iterator;

	public:
		CArray()
		{
			mSize = 0;
		}
		CArray(int nSize, T& t)
		{
			if (nSize > Capacity)
			{
				nSize = Capacity;
			}
			std::fill(begin(), begin() + nSize, t);
			mSize = nSize;
		}
		CArray(int nSize)
		{
			if (nSize > Capacity)
			{
				nSize = Capacity;
			}
			T t = T();
			std::fill(begin(), begin() + nSize, t);
			mSize = nSize;
		}

		template <class InputIterator>
		CArray(InputIterator first, InputIterator last)
		{
			int nSize = last - first;
			if (nSize <= 0)
			{
				return;
			}
			if (nSize > Capacity)
			{
				nSize = Capacity;
			}
			iterator cur = begin();
			for (int i = 0; i < nSize; ++ i)
			{
				*cur++ = *first++;
			}
			mSize = nSize;
		}

		CArray(CArray& rArray)
		{
			std::copy(rArray.begin(), rArray.end(), begin());
			mSize = rArray.mSize;
		}
		~CArray(){}

		CArray& operator=(CArray& rArray)
		{
			std::copy(rArray.begin(), rArray.end(), begin());
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

			std::fill(begin(), (begin() + n), val);
			mSize = n;
		}

		template <typename InputIterator>
		void assign (InputIterator first, InputIterator last)
		{
			int nSize = last - first;
			if (nSize <= 0)
			{
				return;
			}
			iterator cur = begin();
			for (int i = 0; i < nSize && i < mSize; ++ i)
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

			std::copy_backward(position, end(), (end() + 1));
			*position = val;
			++ mSize;
			return position;
		}
		void insert (iterator position, size_type n, const value_type& val)
		{
			if (mSize + n > Capacity)
			{
				return;
			}
			std::copy_backward(position, end(), (begin() + mSize + n));
			std::fill(position, (position + n), val);
			mSize += n;
		}
		void insert (iterator position, iterator first, iterator last)
		{
			int nSize = last - first;
			if (mSize + nSize > Capacity)
			{
				return ;
			}
			std::copy_backward(position, end(), (begin() + mSize + nSize));
			std::copy(first, last, position);

			mSize += nSize;
		}


		iterator erase (iterator position)
		{
			if (position + 1 != end())
			{
				std::copy((position + 1), end(), position);
			}
			-- mSize;
			return position;
		}

		iterator erase (iterator first, iterator last)
		{
			if (last != end())
			{
				std::copy(last, end(), first);
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
		int		mSize;
	};
}

#endif