#ifndef __RANK_H__
#define __RANK_H__
#include "servercommon.h"
#include "fixblockmemory.h"

using namespace Myth;
template<typename T>
class CRankData
{
public:
	int			mRankValue;					// ����ֵ���ȼ���ս���ȣ�
	int			mRankTime;					// ˢ��ʱ��
	int			mRankIndex;					// ����(0,1,2,3,4)
	int			mRankKey;					// ��(���������ID�����߹���ID֮���)
	int			mMemIndex;					// �ڴ������
	T 			mValue;
};


typedef CRankData<int> CRankValue;
template<int MaxRankNum, class T = int>
class CRank
{
public:
	typedef CRankData<T> CRankValueType;
	typedef CFixBlockMemory<CRankValueType, MaxRankNum, MaxRankNum, 0> RANK_MEM_POOL;
	// ����mRankKey, ֵ��mMemIndex����ʱ��map�ɣ�
	// ������һ������������ã����ǵ�ǰ��gcc�汾�����е�����
	typedef map<int, int>	KEY_INDEX_MAP;
public:
	CRank()
	{
		mSortRankNum = 0;
	}
public:
	// ˢ���������
	int refreshRankValue(CRankValueType &rRankValue);

	// ��ȡĳ�����а�����
	CRankValueType *getRankValueByIndex(int nIndex)
	{
		if (nIndex < 0 || nIndex >= mSortRankNum)
		{
			return NULL;
		}
		return mRankDataPool.get(mSortRank[nIndex]);
	}
	
	// ��ȡĳ�����а�����
	CRankValueType *getRankValueByKey(int nRankKey)
	{
		KEY_INDEX_MAP::iterator it = mMapKeyMemIndex.find(nRankKey);
		if (mMapKeyMemIndex.end() == it)
		{
			return NULL;
		}

		return mRankDataPool.get(it->second);
	}

	// ��ȡ�������
	int getRankIndexByKey(int nRankKey);

	// ����������ȡĳ�����а��������
	int getRankKeyByIndex(int nIndex);

	// ��ȡ����
	int GetNum(){ return mSortRankNum; }

	// �Ƚ�����ֵ
	bool CompareRankValue(CRankValueType *pSrcValue, CRankValueType *pDesValue);
	// ���ַ��������ʵ�λ�ã�����std::upper_bound
	int upperBound(int nFirst, int nLast, int nIndex);
private:
	/// �ڴ�أ�ֻ��������������
	RANK_MEM_POOL		mRankDataPool;
	/// �ڴ������ֵ
	int					mSortRank[MaxRankNum];
	/// ���а������Ŀ 
	int					mSortRankNum;
	/// (mRankKey,mMemIndex)��������
	KEY_INDEX_MAP		mMapKeyMemIndex;
};
#include "rank.inl"
#endif