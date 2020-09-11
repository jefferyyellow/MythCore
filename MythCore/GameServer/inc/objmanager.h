#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__
#include "servercommon.h"
#include "fixblockmemory.h"
#include "shareblockmemory.h"
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
		new (pNode)T;
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
		T* pNode = mFixMemory.get(nIndex);
		if (INVALID_OBJ_ID == pNode->getObjID())
		{
			return;
		}
		if (NULL != pNode)
		{
			pNode->setObjID(INVALID_OBJ_ID);
		}

		mFixMemory.free(nIndex);
	}

	void	freeByID(int nObjID)
	{
		int nIndex = calMemIndex(nObjID);
		T* pNode = mFixMemory.get(nIndex);
		if (INVALID_OBJ_ID == pNode->getObjID())
		{
			return;
		}
		if (NULL != pNode)
		{
			pNode->setObjID(INVALID_OBJ_ID);
		}
		mFixMemory.free(nIndex);
	}

	/// 得到最大的obj数目
	int	getMaxNum(){return MaxCount;}

	/// 得到已经用了的Obj数目
	int getObjNum(){return mFixMemory.getObjNum();}

	/// 得到列表头节点
	T*	begin(){return mFixMemory.begin();}

	/// 得到下一个节点
	T*  next(T* pNode){return mFixMemory.next(pNode);}
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

template<typename T, int MaxCount, int IDStart, int IDEnd>
class CShareObjManager
{
public:
	CShareObjManager()
	{
		mLastMaxID = 0;
	}
	~CShareObjManager(){}

public:
	void	init(byte* pMemory)
	{
		mShareMemory.initMemory(pMemory);
	}
public:

	int	getMemorySize()
	{
		return mShareMemory.getMemorySize();
	}
	
	T*		allocObj()
	{
		int nMemIndex = -1;
		T* pNode = mShareMemory.allocate(nMemIndex);
		if (NULL == pNode)
		{
			return NULL;
		}
		new (pNode)T;
		int nID = generalID(nMemIndex);
		pNode->setObjID(nID);
		return pNode;
	}

	T*		getObj(int nObjID)
	{
		int nIndex = calMemIndex(nObjID);
		return mShareMemory.get(nIndex);
	}
	void	free(T *pObj)
	{
		if (NULL == pObj)
		{
			return;
		}

		int nIndex = calMemIndex(pObj->getObjID());
		T* pNode = mShareMemory.get(nIndex);
		if (INVALID_OBJ_ID == pNode->getObjID())
		{
			return;
		}
		if (NULL != pNode)
		{
			pNode->setObjID(INVALID_OBJ_ID);
		}

		mShareMemory.free(nIndex);
	}

	void	freeByID(int nObjID)
	{
		int nIndex = calMemIndex(nObjID);
		T* pNode = mShareMemory.get(nIndex);
		if (INVALID_OBJ_ID == pNode->getObjID())
		{
			return;
		}
		if (NULL != pNode)
		{
			pNode->setObjID(INVALID_OBJ_ID);
		}
		mShareMemory.free(nIndex);
	}

	/// 得到已经分配的Obj数目
	int getObjNum(){ return MaxCount - mShareMemory.GetFreeCount(); }

	/// 得到最大的obj数目
	int	getMaxNum(){ return MaxCount; }
	
	// 得到最小的ID值
	int getMinID(){return IDStart;}
	// 得到最大的ID值
	int getMaxID(){return IDEnd;}

	/// 得到列表头节点
	T*	begin(){ return mShareMemory.begin(); }

	/// 得到下一个节点
	T*  next(T* pNode){ return mShareMemory.next(pNode); }
private:
	// 解释如CObjManager同名函数
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
		if (nID < IDStart || nID >= IDEnd)
		{
			return -1;
		}

		return (nID - IDStart) % MaxCount;
	}


private:
	CShareBlockMemory<T, MaxCount> mShareMemory;
	int mLastMaxID;
};
#endif