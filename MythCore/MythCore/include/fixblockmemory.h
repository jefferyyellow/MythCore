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
	template<typename T, int MaxCount, int BaseCount, int IncreCount>
	class CFixBlockMemory
	{
	public:
		CFixBlockMemory()
		{
			memset(mBlockMemory, 0, sizeof(mBlockMemory));
			mAllocCount = 0;
			mFreeCount= 0;
			allocIncrement(BaseCount);
		}
		~CFixBlockMemory()
		{
			for (int i = 0; i < MaxCount; ++ i)
			{
				if (NULL != mBlockMemory[i])
				{
					delete mBlockMemory[i];
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
					return NULL;
				}
			}
			
			int nIndex = mFreeIndex[mFreeCount - 1];
			if (nIndex < 0 || nIndex >= mAllocCount)
			{
				nMemIndex = -1;
				return NULL;
			}
			
			nMemIndex = nIndex;
			-- mFreeCount;
			return mBlockMemory[nIndex];
		}

		T*			get(int nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= mAllocCount)
			{
				return NULL;
			}
			return mBlockMemory[nMemIndex];
		}

		void		free(int nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= mAllocCount)
			{
				return;
			}
			mFreeIndex[mFreeCount] = nMemIndex;
			++ mFreeCount;
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
				mBlockMemory[i] = new T();
				mFreeIndex[mFreeCount] = nDescrease;
				-- nDescrease;
				++ mFreeCount;
			}
			mAllocCount = nCount;
		}

		int			getAllocCount(){return mAllocCount;}
		int			getObjNum(){return mAllocCount - mFreeCount;}
	private:
		T*			mBlockMemory[MaxCount];
		int			mAllocCount;
		int			mFreeIndex[MaxCount];
		int			mFreeCount;
	};
}
#endif