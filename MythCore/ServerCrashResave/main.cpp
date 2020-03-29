#include "commontype.h"
#include "sharememory.h"
#include "objpool.h"
int main()
{
	CObjPool::Inst();
	int nShareMemorySize = CObjPool::Inst()->getShareMemorySize();
	int nShareMemoryKey = 37345235;
	bool bCreate = true;


	byte* pSharePoint = CShareMemory::createShareMemory(nShareMemoryKey, nShareMemorySize, bCreate);
	if (NULL == pSharePoint)
	{
		return false;
	}

	// ���ԭ��û�иù����ڴ棬������δ�����
	if (bCreate)
	{
		// ɾ���մ����Ĺ����ڴ�Ȼ���˳�
		CShareMemory::destroyShareMemory(nShareMemoryKey);
		return;
	}

	CObjPool::Inst()->setShareMemory(pSharePoint);
	int nMinID = CObjPool::Inst()->getPlayerMinID();
	int nMaxID = CObjPool::Inst()->getPlayerMaxID();
	for (int i = nMinID; i < nMaxID; ++ i)
	{
		CEntityPlayer* pPlayer = CObjPool::Inst()->getObj(i);
		if (NULL == pPlayer)
		{
			return;
		}
		
		if (INVALID_OBJ_ID == pPlayer->getObjID())
		{
			continue;
		}


	}
}