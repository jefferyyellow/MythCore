#ifndef __RANK_H__
#define __RANK_H__
#include "servercommon.h"
#include "fixblockmemory.h"

using namespace Myth;
template<typename T>
class CRankData
{
public:
	int			mRankValue;					// 排行值（等级，战力等）
	int			mRankTime;					// 刷新时间
	int			mRankIndex;					// 排名(0,1,2,3,4)
	int			mRankKey;					// 键(可能是玩家ID，或者公会ID之类的)
	int			mMemIndex;					// 内存池索引
	T 			mValue;
};


typedef CRankData<int> CRankValue;
template<int MaxRankNum, class T = int>
class CRank
{
public:
	typedef CRankData<T> CRankValueType;
	typedef CFixBlockMemory<CRankValueType, MaxRankNum, MaxRankNum, 0> RANK_MEM_POOL;
	// 键是mRankKey, 值是mMemIndex，暂时用map吧，
	// 测试了一下有序数组更好，但是当前的gcc版本编译有点问题
	typedef map<int, int>	KEY_INDEX_MAP;
public:
	CRank()
	{
		mSortRankNum = 0;
	}
public:
	// 刷新玩家数据
	int refreshRankValue(CRankValueType &rRankValue);

	// 获取某项排行榜数据
	CRankValueType *getRankValueByIndex(int nIndex)
	{
		if (nIndex < 0 || nIndex >= mSortRankNum)
		{
			return NULL;
		}
		return mRankDataPool.get(mSortRank[nIndex]);
	}
	
	// 获取某项排行榜数据
	CRankValueType *getRankValueByKey(int nRankKey)
	{
		KEY_INDEX_MAP::iterator it = mMapKeyMemIndex.find(nRankKey);
		if (mMapKeyMemIndex.end() == it)
		{
			return NULL;
		}

		return mRankDataPool.get(it->second);
	}

	// 获取玩家排名
	int getRankIndexByKey(int nRankKey);

	// 根据索引获取某项排行榜玩家数据
	int getRankKeyByIndex(int nIndex);

	// 获取数量
	int GetNum(){ return mSortRankNum; }

	// 比较排行值
	bool CompareRankValue(CRankValueType *pSrcValue, CRankValueType *pDesValue);
	// 二分法搜索合适的位置，类似std::upper_bound
	int upperBound(int nFirst, int nLast, int nIndex);
private:
	/// 内存池，只是用来保存数据
	RANK_MEM_POOL		mRankDataPool;
	/// 内存的索引值
	int					mSortRank[MaxRankNum];
	/// 排行榜里的数目 
	int					mSortRankNum;
	/// (mRankKey,mMemIndex)的索引对
	KEY_INDEX_MAP		mMapKeyMemIndex;
};
#include "rank.inl"
#endif