#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__
#include "commontype.h"
#include "fixblockmemory.h"
using namespace Myth;

template<typename T, int MaxCount, int BaseCount, int IncreCount, int IDStart, int IDEnd>
class CObjManager
{
public:
	CObjManager()
	{
		mLastMaxID = 0;
	}
	~CObjManager(){}

public:
	T*		allocObj()
	{
		int nMemIndex = -1;
		T* pNode = mFixMemory.allocate(nMemIndex);
		if (NULL == pNode)
		{
			return NULL;
		}
		int nID = generalID(nMemIndex);
		pNode->setObjID(nID);
		return pNode;
	}

	T*		getObj(int nObjID)
	{
		int nIndex = calMemIndex(nObjID);
		return mFixMemory.get(nIndex);
	}
	void	free(T *pObj)
	{
		if (NULL == pObj)
		{
			return;
		}

		int nIndex = calMemIndex(pObj->getObjID());
		mFixMemory.free(nIndex);
	}

	void	freeByID(int nObjID)
	{
		int nIndex = calMemIndex(nObjID);
		mFixMemory.free(nIndex);
	}

private:
	// ***********************************************************************
	// 算法注释
	// id_start和id_end之间相关2的24次方,所以是以下的一个方式
	// id_start------------------------------------------------------------------------------------------id_end
	// |------------------------|------------------------|------------------------|------------------------|
	// |-------count------------|-------count------------|-------count------------|-------count------------|
	// 当tmp_id比上次用的id(mLastMaxID)还小的话,就加一个或多个count,为了保持取模的值不变,也为了id的值的增长
	// 为了保证id的向上增长,real_id就是这么计算的,但如果nTempID大于id_end,且nLastID为最终的id,这就完成了一个循环,
	// 所以id不总是向上增长的
	// ***********************************************************************
	int	generalID(int nMemIndex)
	{
		int nLastID = nMemIndex + IDStart;
		if (nLastID <= mLastMaxID)
		{
			int nTempID = nLastID + ((mLastMaxID - nLastID) / MaxCount + 1) * MaxCount;
			// 如果累加的小于最大值才用，不然的话回到最小值
			if (nTempID < IDEnd)
			{
				nLastID = nTempID;
			}
		}

		mLastMaxID = nLastID;
		return nLastID;
	}

	int calMemIndex(int nID)
	{
		if (nID < IDStart || nID >=IDEnd )
		{
			return -1;
		}

		return (nID - IDStart) % MaxCount;
	}

private:
	CFixBlockMemory<T, MaxCount, BaseCount, IncreCount> mFixMemory;
	int mLastMaxID;
};
#endif