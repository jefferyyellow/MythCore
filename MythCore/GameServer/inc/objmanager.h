#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__
#include "servercommon.h"
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

	/// �õ��Ѿ������Obj��Ŀ
	int getObjNum(){ return mFixMemory.getAllocCount(); }

	/// �õ�����obj��Ŀ
	int	getMaxNum(){return MaxCount;}

	/// �õ��Ѿ����˵�Obj��Ŀ
	int getUsedNum(){return mFixMemory.getUsedCount();}
private:
	// ***********************************************************************
	// �㷨ע��
	// id_start��id_end֮�����2��24�η�,���������µ�һ����ʽ
	// id_start------------------------------------------------------------------------------------------id_end
	// |------------------------|------------------------|------------------------|------------------------|
	// |-------count------------|-------count------------|-------count------------|-------count------------|
	// ��tmp_id���ϴ��õ�id(mLastMaxID)��С�Ļ�,�ͼ�һ������count,Ϊ�˱���ȡģ��ֵ����,ҲΪ��id��ֵ������
	// Ϊ�˱�֤id����������,real_id������ô�����,�����nTempID����id_end,��nLastIDΪ���յ�id,��������һ��ѭ��,
	// ����id����������������
	// ***********************************************************************
	int	generalID(int nMemIndex)
	{
		int nLastID = nMemIndex + IDStart;
		if (nLastID <= mLastMaxID)
		{
			int nTempID = nLastID + ((mLastMaxID - nLastID) / MaxCount + 1) * MaxCount;
			// ����ۼӵ�С�����ֵ���ã���Ȼ�Ļ��ص���Сֵ
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