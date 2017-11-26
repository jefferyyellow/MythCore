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