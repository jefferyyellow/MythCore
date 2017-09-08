#include "mapmodule.h"
#include "entity.h"
#include "objpool.h"
#include "scenejob.h"
#include "mapmamager.h"

void CMapModule::OnTimer(unsigned int nTickOffset)
{

}

void CMapModule::broadCastVisiblePlayer(CEntity* pEntity, unsigned short nMessageID, Message* pMessage)
{
	if (NULL == pEntity)
	{
		return;
	}

	CEntity::PLAYER_LIST& rPlayerList = pEntity->getVisiblePlayer();
	CEntity::PLAYER_LIST::iterator it = rPlayerList.begin();

	CEntity* pVisibleEntity = NULL;
	for (; it != rPlayerList.end();)
	{
		pVisibleEntity = (CEntity*)CObjPool::Inst()->getObj(*it);
		if (NULL == pVisibleEntity)
		{
			// ע��������it++
			rPlayerList.eraseNode(it++);
			continue;
		}
		if (emEntityType_Player != pVisibleEntity->getEntityType())
		{
			// ע��������it++
			rPlayerList.eraseNode(it++);
			continue;
		}

		CSceneJob::Inst()->sendClientMessage((CEntityPlayer*)pVisibleEntity, nMessageID, pMessage);
		 ++ it;
	}
}

/// ʵ���ƶ�
void CMapModule::onEntityMove(CEntityCharacter* pEntity, CMythPoint& rDesPos)
{
	if (NULL == pEntity)
	{
		return;
	}

	CMap* pMap = CMapManager::Inst()->getMap(pEntity->getLineID(), pEntity->getMapID(), pEntity->getMapIndex());
	if (NULL == pMap)
	{
		return;
	}
	
	pMap->onEntityMove(pEntity, rDesPos);
}

