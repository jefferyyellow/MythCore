#ifndef __FIXBLOCKMEMORY_H__
#define __FIXBLOCKMEMORY_H__
namespace Myth
{

	/// 突然想到MaxCount定义mBlockMemory[MaxCount]和mFreeIndex[MaxCount]无法动态扩展
	/// 可以分配T的时候，分配T+size(int),这个int为id（或者叫做索引）这个不能生成后不能修改
	/// 就可以动态分配了
	/// class CData
	/// {
	///		int mIndex;
	///		T	mData;
	/// };
	
	// 将分配出去的数据链接成一个双向链表
	template<typename T>
	class CFixBlockData
	{
	public:
		// 数据部分
		T		mData;
		// 前一个数据的索引
		int		mPreIndex;
		// 后一个数据的索引
		int		mNextIndex;
	};

	template<typename T, int MaxCount, int BaseCount, int IncreCount>
	class CFixBlockMemory
	{
		typedef CFixBlockData<T> BLOCK_DATA;

	public:
		CFixBlockMemory()
		{
			memset(mBlockMemory, 0, sizeof(mBlockMemory));
			mAllocCount = 0;
			mFreeCount= 0;
			allocIncrement(BaseCount);
			mHeadIndex = -1;
		}
		~CFixBlockMemory()
		{
			for (int i = 0; i < MaxCount; ++ i)
			{
				if (nullptr != mBlockMemory[i])
				{
					::free(mBlockMemory[i]);
				}
			}
		}

	public:
		T*			allocate(int &nMemIndex)
		{
			if (0 >= mFreeCount)
			{
				allocIncrement(IncreCount);
				if (0 >= mFreeCount)
				{
					return nullptr;
				}
			}
			
			int nIndex = mFreeIndex[mFreeCount - 1];
			if (nIndex < 0 || nIndex >= mAllocCount)
			{
				nMemIndex = -1;
				return nullptr;
			}
			
			nMemIndex = nIndex;
			-- mFreeCount;
			
			// 表示还有其他的节点已经被分配
			if (mHeadIndex >= 0)
			{
				mBlockMemory[mHeadIndex]->mPreIndex = nIndex;
				mBlockMemory[nIndex]->mNextIndex = mHeadIndex;
			}
			mHeadIndex = nIndex;
			new (&(mBlockMemory[nIndex]->mData)) T();
			return &(mBlockMemory[nIndex]->mData);
		}

		T*			get(int nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= mAllocCount)
			{
				return nullptr;
			}
			return &(mBlockMemory[nMemIndex]->mData);
		}

		void		free(int nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= mAllocCount)
			{
				return;
			}
			mFreeIndex[mFreeCount] = nMemIndex;
			++ mFreeCount;
			
			int nPreIndex = mBlockMemory[nMemIndex]->mPreIndex;
			int nNextIndex = mBlockMemory[nMemIndex]->mNextIndex;
			if (nPreIndex >= 0)
			{
				mBlockMemory[nPreIndex]->mNextIndex = nNextIndex;
			}
			if (nNextIndex >= 0)
			{
				mBlockMemory[nNextIndex]->mPreIndex = nPreIndex;
			}
			if (mHeadIndex == nMemIndex)
			{
				mHeadIndex = mBlockMemory[mHeadIndex]->mNextIndex;
			}
			mBlockMemory[nMemIndex]->mNextIndex = -1;
			mBlockMemory[nMemIndex]->mPreIndex = -1;
		}
		
		T*			begin()
		{
			if (mHeadIndex < 0)
			{
				return nullptr;
			}
			return &(mBlockMemory[mHeadIndex]->mData);
		}

		T*			next(T* pValue)
		{
			if (nullptr == pValue)
			{
				return nullptr;
			}
			BLOCK_DATA* pData = (BLOCK_DATA*)pValue;
			if (pData->mNextIndex < 0)
			{
				return nullptr;
			}
			return &(mBlockMemory[pData->mNextIndex]->mData);
		}

		void		allocIncrement(int nSize)
		{
			if (nSize + mAllocCount > MaxCount)
			{
				nSize = MaxCount - mAllocCount;
			}

			if (nSize <= 0)
			{
				return;
			}

			int nCount = mAllocCount + nSize;
			int nDescrease = nCount - 1;
			for (int i = mAllocCount; i < nCount; ++ i)
			{
				mBlockMemory[i] = (BLOCK_DATA*)malloc(sizeof(BLOCK_DATA));
				mBlockMemory[i]->mNextIndex = -1;
				mBlockMemory[i]->mPreIndex = -1;
				mFreeIndex[mFreeCount] = nDescrease;
				-- nDescrease;
				++ mFreeCount;
			}
			mAllocCount = nCount;
		}

		int			getAllocCount(){return mAllocCount;}
		int			getObjNum(){return mAllocCount - mFreeCount;}
	private:
		BLOCK_DATA*			mBlockMemory[MaxCount];
		int					mAllocCount;
		int					mFreeIndex[MaxCount];
		int					mFreeCount;
		int					mHeadIndex;
	};

	template<typename T, int Capacity>
	class CIndexMemoryPool
	{
	public:
		CIndexMemoryPool()
		{
			init();
		}
		~CIndexMemoryPool()
		{
			mFreeCount = 0;
		}

		void		init()
		{
			mFreeCount = Capacity;
			int nIndex = 0;
			for (int i = Capacity - 1; i >= 0; --i)
			{
				mFreeIndex[i] = nIndex;
				++nIndex;
			}
		}

		T*			allocate(int &nMemIndex)
		{
			if (0 >= mFreeCount)
			{
				return nullptr;
			}

			int nIndex = mFreeIndex[mFreeCount - 1];
			if (nIndex < 0 || nIndex >= Capacity)
			{
				nMemIndex = -1;
				return nullptr;
			}

			nMemIndex = nIndex;
			--mFreeCount;
			return &mBlockMemory[nIndex];
		}

		T*			get(int nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= Capacity)
			{
				return nullptr;
			}
			return &mBlockMemory[nMemIndex];
		}

		void		free(int nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= Capacity)
			{
				return;
			}
			mFreeIndex[mFreeCount] = nMemIndex;
			++mFreeCount;
		}

	private:
		T		mBlockMemory[Capacity];
		int		mFreeIndex[Capacity];
		int		mFreeCount;
	};
}
#endif