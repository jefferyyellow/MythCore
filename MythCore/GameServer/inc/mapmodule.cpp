#include "mapmodule.h"
#include "entity.h"
#include "objpool.h"
#include "scenejob.h"
#include "mapmamager.h"
#include "locallogjob.h"

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
			// 注意这里用it++
			rPlayerList.eraseNode(it++);
			continue;
		}
		if (emEntityType_Player != pVisibleEntity->getEntityType())
		{
			// 注意这里用it++
			rPlayerList.eraseNode(it++);
			continue;
		}

		CSceneJob::Inst()->sendClientMessage((CEntityPlayer*)pVisibleEntity, nMessageID, pMessage);
		 ++ it;
	}
}

/// 实体移动
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

/// 处理玩家移动的请求
void CMapModule::onMessagePlayerMoveRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);

}

/// 发送处理玩家移动的回应
void CMapModule::sendPlayerMoveResponse(CEntityPlayer* pPlayer, int nResult)
{
	MYTH_ASSERT(NULL == pPlayer, return);
}

/// 处理玩家传送的消息
void CMapModule::onMessagePlayerTeleportRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
}

// 发送玩家传送回应
void CMapModule::sendPlayerTeleportResponse(CEntityPlayer* pPlayer, int nResult)
{
	MYTH_ASSERT(NULL == pPlayer, return);
}
