#ifndef __INDEXARRAY_H__
#define __INDEXARRAY_H__
#include "fixblockmemory.h"
namespace Myth
{
	
	template<typename T, int Capacity>
	class CIndexArray
	{
	public:
		typedef	T					value_type;
		typedef value_type*			pointer;
		typedef	value_type&			reference;
		typedef	size_t				size_type;
		typedef	ptrdiff_t			difference_type;
		typedef const value_type*	const_pointer;
		typedef const value_type&	const_reference;

	public:
		CIndexArray()
		{
			mSize = 0;
		}
		CIndexArray(int nSize, T& t)
		{
			if (nSize > Capacity)
			{
				nSize = Capacity;
			}
			
			mSize = nSize;
			int nMemIndex = 0;
			for (size_type i = 0; i < nSize; ++ i)
			{
				T* pNewMemory = mMemoryPool.allocate(nMemIndex);
				if (NULL == pNewMemory)
				{ 
					mDataIndex[i] = -1;
					continue;
				}
				mDataIndex[i] = nMemIndex;
				new (pNewMemory)T(t);
			}
		}
		CIndexArray(int nSize)
		{
			if (nSize > Capacity)
			{
				nSize = Capacity;
			}
			T t = T();
			mSize = nSize;
			int nMemIndex = 0;
			for (size_type i = 0; i < nSize; ++i)
			{
				T* pNewMemory = mMemoryPool.allocate(nMemIndex);
				if (NULL == pNewMemory)
				{
					mDataIndex[i] = -1;
					continue;
				}
				mDataIndex[i] = nMemIndex;
				new (pNewMemory)T(t);
			}
		}

		template <class InputIterator>
		CIndexArray(InputIterator first, InputIterator last)
		{
			size_type nSize = last - first;
			if (nSize <= 0)
			{
				return;
			}
			if (nSize > Capacity)
			{
				nSize = Capacity;
			}
			mSize = nSize;
			int nMemIndex = 0;
			for (size_type i = 0; i < nSize; ++i)
			{
				T* pNewMemory = mMemoryPool.allocate(nMemIndex);
				if (NULL == pNewMemory)
				{
					mDataIndex[i] = -1;
					continue;
				}
				mDataIndex[i] = nMemIndex;
				new (pNewMemory)T(*first++);
			}
		}

		CIndexArray(CIndexArray& rArray)
		{
			mSize = rArray.mSize;
			int nMemIndex = 0;
			for (size_type i = 0; i < mSize; ++i)
			{
				T* pNewMemory = mMemoryPool.allocate(nMemIndex);
				if (NULL == pNewMemory)
				{
					mDataIndex[i] = -1;
					continue;
				}
				mDataIndex[i] = nMemIndex;
				new (pNewMemory)T(rArray[i]);
			}
		}

		void init()
		{
			mMemoryPool.init();
			mSize = 0;
		}

		~CIndexArray()
		{
			clear();
		}

		CIndexArray& operator=(CIndexArray& rArray)
		{
			if (&rArray == this)
			{
				return;
			}
			mSize = rArray.mSize;
			int nMemIndex = 0;
			
			for (size_type i = 0; i < mSize; ++i)
			{
				T* pNewMemory = mMemoryPool.allocate(nMemIndex);
				if (NULL == pNewMemory)
				{
					mDataIndex[i] = -1;
					continue;
				}
				mDataIndex[i] = nMemIndex;
				new (pNewMemory)T(rArray[i]);
			}
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
			return *mMemoryPool.get(mDataIndex[n]);
		}

		const_reference operator[] (size_type n) const
		{
			return *mMemoryPool.get(mDataIndex[n]);
		}

		void assign(int n, const value_type& val)
		{
			assign(size_type(n), val);
		}

		void assign(size_type n, const value_type& val)
		{
			if (n > Capacity)
			{
				return;
			}
			mSize = n;

			int nMemIndex = 0;
			for (size_type i = 0; i < mSize; ++i)
			{
				T* pNewMemory = mMemoryPool.allocate(nMemIndex);
				if (NULL == pNewMemory)
				{
					mDataIndex[i] = -1;
					continue;
				}
				mDataIndex[i] = nMemIndex;
				new (pNewMemory)T(val);
			}
		}

		void setSize(size_type n)
		{
			if (n > Capacity)
			{
				return;
			}
			clear();
			mSize = n;

			int nMemIndex = 0;
			for (size_type i = 0; i < mSize; ++i)
			{
				T* pNewMemory = mMemoryPool.allocate(nMemIndex);
				if (NULL == pNewMemory)
				{
					mDataIndex[i] = -1;
					continue;
				}
				mDataIndex[i] = nMemIndex;
			}
		}

		void push_back(const value_type& val)
		{
			if (mSize >= Capacity)
			{
				return;
			}
			int nMemIndex = 0;
			T* pNewMemory = mMemoryPool.allocate(nMemIndex);
			if (NULL == pNewMemory)
			{
				return;
			}
			new (pNewMemory)T(val);
			mDataIndex[mSize] = nMemIndex;
			++mSize;
		}

		void pop_back()
		{
			if (mSize > 0)
			{
				mMemoryPool.free(mDataIndex[mSize]);
			}
			--mSize;
		}

		void insert(size_type pos, size_type n, const value_type& val)
		{
			if (mSize + n > Capacity)
			{
				return;
			}
			std::copy_backward(mDataIndex + pos, mDataIndex + mSize, mDataIndex + mSize + n);
			mSize += n;
			int nMemIndex = 0;
			for (size_type i = 0; i < n; ++i)
			{
				T* pNewMemory = mMemoryPool.allocate(nMemIndex);
				if (NULL == pNewMemory)
				{
					mDataIndex[pos] = -1;
					continue;
				}
				mDataIndex[pos] = nMemIndex;
				++ pos;
				new (pNewMemory)T(val);
			}

		}

		void insert(size_type pos, const value_type& val)
		{
			if (mSize >= Capacity)
			{
				return ;
			}
			insert(pos, 1, val);
		}

		void erase(size_type pos, size_type n)
		{
			if (pos + n > mSize)
			{
				return;
			}
			for (size_type i = pos; i < pos + n; ++ i)
			{
				mMemoryPool.free(mDataIndex[i]);
			}
			if (pos + n < mSize)
			{
				size_type* pSrcIndex = mDataIndex +  pos + n;
				size_type* pDstIndex = mDataIndex +  pos;
				size_type* pEndIndex = mDataIndex + mSize;
				while (pSrcIndex != pEndIndex)
				{
					*pDstIndex ++ = *pSrcIndex ++;
				}
			}
			mSize -= n;
		}

		void erase(size_type pos)
		{
			erase(pos, 1);
		}

		void clear()
		{
			for (size_type i = 0; i < mSize; ++ i)
			{
				mMemoryPool.free(mDataIndex[i]);
			}
			mSize = 0;
		}
	private:
		size_type	mDataIndex[Capacity];
		size_type	mSize;
		CIndexMemoryPool<T, Capacity>	mMemoryPool;
	};
}

#endif