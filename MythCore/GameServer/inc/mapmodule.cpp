#include "mapmodule.h"
#include "mapmodule.hxx.pb.h"
#include "entity.h"
#include "objpool.h"
#include "scenejob.h"
#include "mapmamager.h"
#include "locallogjob.h"
#include "entityplayer.h"
#include "mapmodule.hxx.pb.h"
#include "mapconfigmanager.h"
#include "errcode.h"

void CMapModule::OnTimer(unsigned int nTickOffset)
{

}

int CMapModule::teleportEntity(CEntity* pEntity, unsigned short nMapID, CMythPoint& rPos)
{
	if (NULL == pEntity)
	{
		return -1;
	}
	CMap* pMap = CMapManager::Inst()->getMap(pEntity->getLineID(), nMapID, pEntity->getMapIndex());
	if (NULL == pMap)
	{
		return -1;
	}
	if (!pMap->removeEntityFromMap(pEntity))
	{
		LOG_ERROR("remove entity from map failure, EntityID: %d, PosX: %d, PosY: %d", pEntity->getObjID(), rPos.mX, rPos.mY);
		return -1;
	}

	if (!pMap->createEntityToMap(pEntity, rPos))
	{
		LOG_ERROR("create entity into map failure, EntityID: %d, PosX: %d, PosY: %d", pEntity->getObjID(), rPos.mX, rPos.mY);
		return -1;
	}
	pEntity->setMapID(nMapID);
	pEntity->setPos(rPos);
	return SUCCESS;
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

/// ��������ƶ�������
void CMapModule::onMessagePlayerMoveRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	CMessagePlayerMoveRequest* pPlayerMoveRequest = (CMessagePlayerMoveRequest*)pMessage;
	MYTH_ASSERT(NULL == pPlayerMoveRequest, return)

	CMythPoint tPos((int)pPlayerMoveRequest->desposx(), (int)pPlayerMoveRequest->desposy());
	onEntityMove(pPlayer, tPos);
}

/// ���ʹ�������ƶ��Ļ�Ӧ
void CMapModule::sendPlayerMoveResponse(CEntityPlayer* pPlayer, int nResult)
{
	MYTH_ASSERT(NULL == pPlayer, return);
	CMessagePlayerMoveResponse tPlayerMoveResponse;
	tPlayerMoveResponse.set_result(nResult);

	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_RESPONSE_PLAYER_MOVE, &tPlayerMoveResponse);
}

/// ������Ҵ��͵���Ϣ
void CMapModule::onMessagePlayerTeleportRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	CMessagePlayerTeleportRequest* pPlayerTeleportRequest = (CMessagePlayerTeleportRequest*)pMessage;
	MYTH_ASSERT(NULL == pPlayerTeleportRequest, return);

	unsigned short nMapID = pPlayerTeleportRequest->mapid();
	CMythPoint tPoint;
	CMapConfigManager::Inst()->getMapPortalPos(pPlayer->getMapID(), nMapID, tPoint);
	if (SUCCESS != teleportEntity(pPlayer, nMapID, tPoint))
	{
		sendPlayerTeleportResponse(pPlayer, ERROR_MAP_TELEPORT_MAP_FAILURE);
		return;
	}

	sendPlayerTeleportResponse(pPlayer, SUCCESS);
}

// ������Ҵ��ͻ�Ӧ
void CMapModule::sendPlayerTeleportResponse(CEntityPlayer* pPlayer, int nResult)
{
	MYTH_ASSERT(NULL == pPlayer, return);

	CMessagePlayerTeleportResponse tPlayerTeleportResponse;
	tPlayerTeleportResponse.set_result(nResult);
	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_RESPONSE_PLAYER_TELEPORT, &tPlayerTeleportResponse);
}