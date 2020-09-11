#ifndef __SHAREBLOCKMEMORY_H__
#define __SHAREBLOCKMEMORY_H__
namespace Myth
{
	/// ��Ҫ�����ע�����ڹ���ShareMemory���ڴ棬����û�ж�̬���䣬Ҳû���ͷ�
	// �������ȥ���������ӳ�һ��˫������
	template<typename T>
	class CShareBlockData
	{
	public:
		// ���ݲ���
		T		mData;
		// ǰһ�����ݵ�����
		int		mPreIndex;
		// ��һ�����ݵ�����
		int		mNextIndex;
	};

	template<typename T, int MaxCount>
	class CShareBlockMemory
	{
		typedef CShareBlockData<T> BLOCK_DATA;
	public:
		CShareBlockMemory()
		{
			memset(mBlockMemory, 0, sizeof(mBlockMemory));
			mFreeCount= 0;
			mHeadIndex = -1;
		}
		~CShareBlockMemory()
		{
		}

	public:
		void		initMemory(byte* pShareMemory)
		{
			BLOCK_DATA* pMemory = (BLOCK_DATA*)pShareMemory;
			for (int i = 0; i < MaxCount; ++ i)
			{
				mFreeIndex[i] = i;
				mBlockMemory[i] = pMemory;
				pMemory->mNextIndex = -1;
				pMemory->mPreIndex = -1;
				++ pMemory;
			}
			mFreeCount = MaxCount;
			mHeadIndex = -1;
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

			// ��ʾ���������Ľڵ��Ѿ�������
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
			if (nMemIndex < 0 || nMemIndex >= MaxCount)
			{
				return NULL;
			}
			return &(mBlockMemory[nMemIndex]->mData);
		}

		void		free(int nMemIndex)
		{
			if (nMemIndex < 0 || nMemIndex >= MaxCount)
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
				return NULL;
			}
			return &(mBlockMemory[mHeadIndex]->mData);
		}

		T*			next(T* pValue)
		{
			if (NULL == pValue)
			{
				return NULL;
			}
			BLOCK_DATA* pData = (BLOCK_DATA*)pValue;
			if (pData->mNextIndex < 0)
			{
				return NULL;
			}
			return &(mBlockMemory[pData->mNextIndex]->mData);
		}


		int			GetFreeCount(){return mFreeCount;}
		void		SetFreeCount(int nFreeCount){mFreeCount = nFreeCount;}

		int			getMemorySize(){return sizeof(BLOCK_DATA) * MaxCount;}

	private:
		BLOCK_DATA*			mBlockMemory[MaxCount];
		int					mFreeIndex[MaxCount];
		int					mFreeCount;
		int					mHeadIndex;
	};
}
#endif