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
#include "entitycreator.h"

CMapModule::CMapModule()
	:mDestroyTimer(CHECK_AUTO_DESTORY_MAP_TIME)
{

}

/// 启动服务器
void CMapModule::onLaunchServer()
{
	CMapConfigManager::Inst()->loadMapConfig("gameserverconfig/map/maplist.xml");
}

/// 启动完成检查
bool CMapModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CMapModule::onLaunchComplete()
{

}

/// 退出服务器
void CMapModule::onExitServer()
{

}

/// 新一天到来
void CMapModule::onNewDayCome()
{

}

/// 新一周到来
void CMapModule::onNewWeekCome()
{

}

/// 建立实体
void CMapModule::onCreatePlayer(CEntityPlayer* pPlayer)
{
	CMap* pMap = CMapManager::Inst()->getMap(pPlayer->getLineID(), pPlayer->getMapID(), pPlayer->getMapIndex());
	// 如果地图不存在，检查是不是自动销毁的地图，如果是，重新创建一张
	if (NULL == pMap)
	{
		CMapConfig* pMapConfig = CMapConfigManager::Inst()->getMapConfig(pPlayer->getMapID());
		if (NULL == pMapConfig)
		{
			LOG_ERROR("find map config error, line id: %d, map id: %d, map index: %d", pPlayer->getLineID(), pPlayer->getMapID(), pPlayer->getMapIndex());
			return;
		}
		if (pMapConfig->getAutoDestory())
		{
			pMap = createMapFromConfig(pPlayer->getLineID(), pPlayer->getMapID(), pPlayer->getMapIndex());
		}
	}
	
	if (NULL == pMap)
	{
		LOG_ERROR("the map of the player's location is null , line id: %d, map id: %d, map index: %d", pPlayer->getLineID(), pPlayer->getMapID(), pPlayer->getMapIndex());
		return;
	}

	pMap->createPlayer(pPlayer);
}

/// 销毁实体
void CMapModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{
	CMap* pMap = CMapManager::Inst()->getMap(pPlayer->getLineID(), pPlayer->getMapID(), pPlayer->getMapIndex());
	if (NULL == pMap)
	{
		return;
	}
	pMap->destroyPlayer(pPlayer);
}

void CMapModule::onTimer(unsigned int nTickOffset)
{
	if (mDestroyTimer.elapse(nTickOffset))
	{
		CMapManager::Inst()->checkAutoDestoryMap();
	}
}

void CMapModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
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
		// 如果是玩家,检查是不是自动销毁的地图，如果是的，自动创建一张新的地图
		if (pEntity->isPlayer())
		{
			CMapConfig* pMapConfig = CMapConfigManager::Inst()->getMapConfig(pEntity->getMapID());
			if (NULL == pMapConfig)
			{
				LOG_ERROR("find map config error, line id: %d, map id: %d, map index: %d", pEntity->getLineID(), pEntity->getMapID(), pEntity->getMapIndex());
				return -1;
			}
			if (pMapConfig->getAutoDestory())
			{
				pMap = createMapFromConfig(pEntity->getLineID(), pEntity->getMapID(), pEntity->getMapIndex());
			}
		}
	}
	if (NULL == pMap)
	{
		LOG_ERROR("target map is null, line id: %d, map id: %d, map index: %d", pEntity->getLineID(), pEntity->getMapID(), pEntity->getMapIndex());
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

/// 创建实体
CEntity* CMapModule::createEntity(CEntityCreator* pCreator)
{
	if (NULL == pCreator)
	{
		return NULL;
	}

	CMap* pMap = CMapManager::Inst()->getMap(pCreator->mLineID, pCreator->mMapID, pCreator->mMapIndex);
	if (NULL == pMap)
	{
		return NULL;
	}
	CEntity* pEntity = pMap->createEntity(pCreator);

	return pEntity;
}

/// 删除实体
void CMapModule::destroyEntity(CEntity* pEntity)
{
	CMap* pMap = CMapManager::Inst()->getMap(pEntity->getLineID(), pEntity->getMapID(), pEntity->getMapIndex());
	if (NULL == pMap)
	{
		return;
	}

	pMap->destroyEntity(pEntity);
}

/// 根据地图线ID，地图ID，地图索引创建地图
CMap* CMapModule::createMapFromConfig(unsigned short nLineID, unsigned short nMapID, int nMapIndex)
{
	CMapConfig* pMapConfig = CMapConfigManager::Inst()->getMapConfig(nMapID);
	if (NULL == pMapConfig)
	{
		return NULL;
	}

	CMap* pMap = CMapManager::Inst()->createMap(nLineID, nMapID, nMapIndex, pMapConfig->getLength(), pMapConfig->getWidth());
	if (NULL == pMap)
	{
		return NULL;
	}

	pMapConfig->createMapFromConfig(pMap);
	return pMap;
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
		pVisibleEntity = static_cast<CEntity*>(CObjPool::Inst()->getObj(*it));
		if (NULL == pVisibleEntity)
		{
			// 注意这里用it++
			rPlayerList.eraseNode(it++);
			continue;
		}
		if (!pVisibleEntity->isPlayer())
		{
			// 注意这里用it++
			rPlayerList.eraseNode(it++);
			continue;
		}

		CSceneJob::Inst()->send2Player(static_cast<CEntityPlayer*>(pVisibleEntity), nMessageID, pMessage);
		 ++ it;
	}
}

/// 广播地图上所有玩家
void CMapModule::broadCastMapPlayer(CEntity* pEntity, unsigned short nMessageID, Message* pMessage)
{
	CMap* pMap = CMapManager::Inst()->getMap(pEntity->getLineID(), pEntity->getMapID(), pEntity->getMapIndex());
	if (NULL == pMap)
	{
		return;
	}

	CMap::PLAYER_LIST& rPlayerList = pMap->getPlayerList();
	CMap::PLAYER_LIST::iterator it = rPlayerList.begin();
	for (; it != rPlayerList.end(); ++ it)
	{
		CEntity* pEntity = static_cast<CEntity*>(CObjPool::Inst()->getObj(*it));
		if (NULL == pEntity)
		{
			LOG_ERROR("entity data is null, entity id: %d", *it);
			continue;
		}
		if (!pEntity->isPlayer())
		{
			LOG_ERROR("entity is not a player, entity id: %d", *it);
			continue;
		}

		CSceneJob::Inst()->send2Player(static_cast<CEntityPlayer*>(pEntity), nMessageID, pMessage);
	}
}


/// 实体移动
void CMapModule::onEntityMove(CEntity* pEntity, CMythPoint& rDesPos)
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
	CPlayerMoveRequest* pPlayerMoveRequest = (CPlayerMoveRequest*)pMessage;
	MYTH_ASSERT(NULL == pPlayerMoveRequest, return)

	CMythPoint tPos((int)pPlayerMoveRequest->desposx(), (int)pPlayerMoveRequest->desposy());
	onEntityMove(pPlayer, tPos);
}

/// 发送处理玩家移动的回应
void CMapModule::sendPlayerMoveResponse(CEntityPlayer* pPlayer, int nResult)
{
	MYTH_ASSERT(NULL == pPlayer, return);
	CPlayerMoveResponse tPlayerMoveResponse;
	tPlayerMoveResponse.set_result(nResult);

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_RESPONSE_PLAYER_MOVE, &tPlayerMoveResponse);
}

/// 处理玩家传送的消息
void CMapModule::onMessagePlayerTeleportRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	CPlayerTeleportRequest* pPlayerTeleportRequest = (CPlayerTeleportRequest*)pMessage;
	MYTH_ASSERT(NULL == pPlayerTeleportRequest, return);

	unsigned short nMapID = pPlayerTeleportRequest->mapid();
	CMythPoint tPoint;
	CMapConfigManager::Inst()->getMapPortalPos(pPlayer->getMapID(), nMapID, tPoint);
	if (SUCCESS != teleportEntity(pPlayer, nMapID, tPoint))
	{
		sendPlayerTeleportResponse(pPlayer, ERR_MAP_TELEPORT_MAP_FAILURE);
		return;
	}

	sendPlayerTeleportResponse(pPlayer, SUCCESS);
}

// 发送玩家传送回应
void CMapModule::sendPlayerTeleportResponse(CEntityPlayer* pPlayer, int nResult)
{
	MYTH_ASSERT(NULL == pPlayer, return);

	CPlayerTeleportResponse tPlayerTeleportResponse;
	tPlayerTeleportResponse.set_result(nResult);
	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_RESPONSE_PLAYER_TELEPORT, &tPlayerTeleportResponse);
}
