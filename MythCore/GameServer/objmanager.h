#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__
#include "commontype.h"
#include "fixblockmemory.h"
using namespace Myth;

template<typename T, sint32 MaxCount, sint32 BaseCount, sint32 IncreCount, uint32 IDStart, uint32 IDEnd>
class CObjManager
{
public:
	CObjManager(){}
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
		uint32 nID = generalID(nMemIndex);
		pNode->setObjID(nID);
		return pNode;
	}

	T*		getObj(uint32 nObjID)
	{
		sint32 nIndex = calMemIndex(nObjID);
		return mFixMemory.get(nIndex);
	}
	void	free(T *pObj)
	{
		if (NULL == pObj)
		{
			return;
		}

		sint32 nIndex = calMemIndex(pObj->getObjID());
		mFixMemory.free(nIndex);
	}

	void	freeByID(uint32 nObjID)
	{
		sint32 nIndex = calMemIndex(nObjID);
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
	uint32	generalID(sint32 nMemIndex)
	{
		uint32 nLastID = nMemIndex + IDStart;
		if (nLastID <= mLastMaxID)
		{
			uint32 nTempID = nLastID + ((mLastMaxID - nLastID) / MaxCount + 1) * MaxCount;
			// ����ۼӵ�С�����ֵ���ã���Ȼ�Ļ��ص���Сֵ
			if (nTempID < IDEnd)
			{
				nLastID = nTempID;
			}
		}

		mLastMaxID = nLastID;
		return nLastID;
	}

	sint32 calMemIndex(uint32 nID)
	{
		if (nID < IDStart || nID >=IDEnd )
		{
			return -1;
		}

		return (nID - IDStart) / MaxCount;
	}

private:
	CFixBlockMemory<T, MaxCount, BaseCount, IncreCount> mFixMemory;
	uint32 mLastMaxID;
};
#endif