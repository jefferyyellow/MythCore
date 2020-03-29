#ifndef __SHAREBLOCKMEMORY_H__
#define __SHAREBLOCKMEMORY_H__
namespace Myth
{
	/// 主要这个类注意用于管理ShareMemory的内存，所以没有动态分配，也没有释放
	template<typename T, int MaxCount>
	class CShareBlockMemory
	{
	public:
		CShareBlockMemory()
		{
			memset(mBlockMemory, 0, sizeof(mBlockMemory));
			mFreeCount= 0;
		}
		~CShareBlockMemory()
		{
		}

	public:
		void		initMemory(byte* pShareMemory)
		{
			T* pMemory = (T*)pShareMemory;
			for (int i = 0; i < MaxCount; ++ i)
			{
				mBlockMemory[i] = pMemory;
				++ pMemory;
			}
		}

		T*			allocate(int &nMemIndex)
		{
			if (0 >= mFreeCount)
			{
				return NULL;
			}

			int nIndex = mFreeIndex[mFreeCount - 1];
			if (nIndex < 0 || nIndex >= MaxCount)
			{
				nMemIndex = -1;
				return NULL;
			}

			nMemIndex = nIndex;
			--mFreeCount;
			return mBlockMemory[nIndex];
		}

		T*			get(int nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= MaxCount)
			{
				return NULL;
			}
			return mBlockMemory[nMemIndex];
		}

		void		free(int nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= MaxCount)
			{
				return;
			}
			mFreeIndex[mFreeCount] = nMemIndex;
			++ mFreeCount;
		}

		int			GetFreeCount(){return mFreeCount;}
		void		SetFreeCount(int nFreeCount){mFreeCount = nFreeCount;}

	private:
		T*			mBlockMemory[MaxCount];
		int			mFreeIndex[MaxCount];
		int			mFreeCount;
	};
}
#endif