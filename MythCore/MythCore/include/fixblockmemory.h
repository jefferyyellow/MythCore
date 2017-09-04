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
	template<typename T, uint MaxCount, uint BaseCount, uint IncreCount>
	class CFixBlockMemory
	{
	public:
		CFixBlockMemory()
		{
			memset(mBlockMemory, 0, sizeof(mBlockMemory));
			mAllocCount = 0;
			mFreeCount= 0;
		}
		~CFixBlockMemory()
		{
			for (int i = 0; i < MaxCount;)
			{
				if (NULL != mBlockMemory[i])
				{
					delete mBlockMemory[i];
				}
			}
		}

	public:
		T*			allocate(sint &nMemIndex)
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
			return mBlockMemory[nIndex];
		}

		T*			get(sint nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= mAllocCount)
			{
				return NULL;
			}
			return mBlockMemory[nMemIndex];
		}

		void		free(sint nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= mAllocCount)
			{
				return;
			}
			mFreeIndex[mFreeCount] = nMemIndex;
		}

		void		allocIncrement(uint nSize)
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
			for (int i = mAllocCount; i < nCount; ++ i)
			{
				mBlockMemory[i] = new T();
				mFreeIndex[mFreeCount] = i;
				++ mFreeCount;
			}
			mAllocCount = nCount;
		}
	
	private:
		T*			mBlockMemory[MaxCount];
		sint32		mAllocCount;
		int			mFreeIndex[MaxCount];
		sint32		mFreeCount;
	};
}
#endif