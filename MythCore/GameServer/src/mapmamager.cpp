#include "mapmamager.h"
#include "errcode.h"
#include "locallogjob.h"
#include "entity.h"
#include "entityplayer.h"
#include "mapregionsearch.h"
#include "mapmodule.hxx.pb.h"
#include "scenejob.h"
#include "mapconfigmanager.h"
#include "template.h"
#include "entitycreator.h"
#include "timemanager.h"
CMapCell::ENTITY_ALLOC CMapCell::mEntityAlloc;
void CMapCell::pushEntity(int nObjID)
{
	CShareList<int>::CShareListNode<int>* pNode = mEntityAlloc.allocate();
	pNode->mData = nObjID;
	pNode->mpNext = NULL;
	pNode->mpPrev = NULL;
	mEntityList.push_front(pNode);
}

void CMapCell::removeEntity(int nObjID)
{
	ENTITY_LIST::iterator it = mEntityList.begin();
	for (; it != mEntityList.end(); ++ it)
	{
		if (*it == nObjID)
		{
			mEntityList.eraseNode(it);
			mEntityAlloc.free(it.mNode);
			break;
		}
	}
}

bool CMapCell::checkEntity(int nObjID)
{
	ENTITY_LIST::iterator it = mEntityList.begin();
	for (; it != mEntityList.end(); ++it)
	{
		if (*it == nObjID)
		{
			return true;
		}
	}
	return false;
}

int CMapCell::getEntityNum()
{
	int nEntityNum = 0;
	ENTITY_LIST::iterator it = mEntityList.begin();
	for (; it != mEntityList.end(); ++it)
	{
		++ nEntityNum;
	}

	return nEntityNum;
}

int CMap::initSize(short nLength, short nWidth)
{
	mMapCell = new CMapCell[nLength * nWidth];
	if (NULL == mMapCell)
	{
		LOG_ERROR("create map unit failure!");
		return ERR_MAP_CREATE_MAP_UNIT_FAILURE;
	}

	mLength = nLength;
	mWidth = nWidth;
	return SUCCESS;
}

void CMap::clear()
{
	if (NULL != mMapCell)
	{
		delete []mMapCell;
		mMapCell = NULL;
	}
}

/// 得到可以区域
void CMap::getVisibleRect(CEntity* pEntity, CMythRect& rRect)
{
	rRect.mTopLeft.mX = (std::max)(pEntity->getPos().mX - VISIBLE_RADIUS_X, 0);
	rRect.mTopLeft.mY = (std::max)(pEntity->getPos().mY - VISIBLE_RADIUS_Y, 0);

	rRect.mBottomRight.mX = (std::min)(pEntity->getPos().mX + VISIBLE_RADIUS_X, getLength() - 1);
	rRect.mBottomRight.mY = (std::min)(pEntity->getPos().mY + VISIBLE_RADIUS_Y, getWidth() - 1);
	return;
}

/// 实体移动
void CMap::onEntityMove(CEntity* pEntity, CMythPoint& rDesPos)
{
	MYTH_ASSERT(NULL != pEntity, return);
	// 已经在目的地了
	if (pEntity->getPos() == rDesPos)
	{
		return;
	}

	CMythRect tSrcRect;
	getVisibleRect(pEntity, tSrcRect);
	
	// 从原来的地图单元里移除
	if (!removeEntityFromMapCell(pEntity))
	{
		return;
	}

	pEntity->setPos(rDesPos);
	CMythRect tDesRect;
	getVisibleRect(pEntity, tDesRect);
	// 从原来的地图单元移除触发
	onRemoveEntityFromMapCell(pEntity, tSrcRect, tDesRect);
	// 加入到新的地图单元
	addEntityToMapCell(pEntity);
	// 加入到新的地图单元触发
	onAddEntityToMapCell(pEntity, tSrcRect, tDesRect);
}


/// 把实体加入地图单元
bool CMap::addEntityToMapCell(CEntity* pEntity)
{
	MYTH_ASSERT(NULL != pEntity, return false);

	CMapCell* pMapCell = getMapCell(pEntity->getPosX(), pEntity->getPosY());
	if (NULL == pMapCell)
	{
		return false;
	}

	pMapCell->pushEntity(pEntity->getObjID());
	return true;
}

/// 把实体加入地图单元触发
void CMap::onAddEntityToMapCell(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect)
{
	CMythRect tRectArray[4];
	// tDesRect - tSrcRect,从新增的那一部分中增加
	int nRectNum = rSrcRect.subtractRect(rDesRect, tRectArray);
	if (nRectNum <= 0)
	{
		return;
	}

	CMapRegionSearch tSearch;
	for (int i = 0; i < nRectNum; ++ i)
	{
		tSearch.searchEntity(this, tRectArray[i]);
	}

	// 玩家
	if (emEntityType_Player == pEntity->getEntityType())
	{
		// 告诉周边的玩家创建该玩家
		createPlayerList2Player(tSearch.mPlayerList, static_cast<CEntityPlayer*>(pEntity));
		// 告诉该玩家创建周边玩家
		createPlayer2PlayerList(static_cast<CEntityPlayer*>(pEntity), tSearch.mPlayerList);
		// 告诉该玩家创建NPC
		createNPCList2Player(tSearch.mNPCList, static_cast<CEntityPlayer*>(pEntity));
	}
	else
	{
		// 告诉周边的玩家创建该NPC
		createNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// 将实体从地图单元中移除
bool CMap::removeEntityFromMapCell(CEntity* pEntity)
{
	MYTH_ASSERT(NULL != pEntity, return false);

	CMapCell* pMapCell = getMapCell(pEntity->getPosX(), pEntity->getPosY());
	if (NULL == pMapCell)
	{
		return false;
	}
	pMapCell->removeEntity(pEntity->getObjID());
	return true;
}

/// 将实体从地图单元中移除触发
void CMap::onRemoveEntityFromMapCell(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect)
{
	MYTH_ASSERT(NULL != pEntity, return);
	CMythRect tRectArray[4];
	// tSrcRect - tDesRect,从原来的那一部分中删除
	int nRectNum = rDesRect.subtractRect(rSrcRect, tRectArray);
	if (nRectNum <= 0)
	{
		return;
	}

	CMapRegionSearch tSearch;
	for (int i = 0; i < nRectNum; ++i)
	{
		tSearch.searchEntity(this, tRectArray[i]);
	}

	// 玩家
	if (emEntityType_Player == pEntity->getEntityType())
	{
		// 通知周边玩家移除该玩家
		destroyPlayer2PlayerList(static_cast<CEntityPlayer*>(pEntity), tSearch.mPlayerList);
		// 通知该玩家移除周边玩家
		destroyPlayerList2Player(tSearch.mPlayerList, static_cast<CEntityPlayer*>(pEntity));
		// 通知该玩家移除周边NPC列表
		destroyNPCList2Player(tSearch.mNPCList, static_cast<CEntityPlayer*>(pEntity));
	}
	else
	{
		// 通知周边玩家移除该NPC
		destroyNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// 在地图中创建实体
bool CMap::createEntityToMap(CEntity* pEntity, CMythPoint& rPos)
{
	if (NULL == pEntity)
	{
		return false;
	}
	CMapCell* pMapCell = getMapCell(rPos.mX, rPos.mY);
	if (NULL == pMapCell)
	{
		return false;
	}

	pMapCell->pushEntity(pEntity->getObjID());
	onCreateEntityToMap(pEntity);
	if (pEntity->isPlayer())
	{
		mPlayerList.push_back(pEntity->getObjID());
	}
	else
	{
		mEntityList.push_back(pEntity->getObjID());
	}
	return true;
}

/// 在地图中创建实体触发
void CMap::onCreateEntityToMap(CEntity* pEntity)
{
	if (NULL == pEntity)
	{
		return;
	}

	CMythRect tRect;
	getVisibleRect(pEntity, tRect);

	CMapRegionSearch tSearch;
	tSearch.searchEntity(this, tRect);

	// 玩家
	if (emEntityType_Player == pEntity->getEntityType())
	{
		// 告诉周边的玩家创建该玩家
		createPlayerList2Player(tSearch.mPlayerList, static_cast<CEntityPlayer*>(pEntity));
		// 告诉该玩家创建周边玩家
		createPlayer2PlayerList(static_cast<CEntityPlayer*>(pEntity), tSearch.mPlayerList);
		// 告诉该玩家创建NPC
		createNPCList2Player(tSearch.mNPCList, static_cast<CEntityPlayer*>(pEntity));
	}
	else
	{
		// 告诉周边的玩家创建该NPC
		createNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// 在地图中移除实体
bool CMap::removeEntityFromMap(CEntity* pEntity)
{
	MYTH_ASSERT(NULL != pEntity, return false);

	CMapCell* pMapCell = getMapCell(pEntity->getPosX(), pEntity->getPosY());
	if (NULL == pMapCell)
	{
		return false;
	}
	pMapCell->removeEntity(pEntity->getObjID());
	onRemoveEntityFromMap(pEntity);
	if (pEntity->isPlayer())
	{
		PLAYER_LIST::iterator it = mPlayerList.begin();
		for (; it != mPlayerList.end(); ++it)
		{
			if (*it == pEntity->getObjID())
			{
				mPlayerList.erase(it);
				break;
			}
		}
	}
	else
	{
		ENTITY_LIST::iterator it = mEntityList.begin();
		for (; it != mEntityList.end(); ++it)
		{
			if (*it == pEntity->getObjID())
			{
				mEntityList.erase(it);
				break;
			}
		}
	}
	return true;
}


/// 在地图中移除实体触发
void CMap::onRemoveEntityFromMap(CEntity* pEntity)
{
	if (NULL == pEntity)
	{
		return;
	}

	CMythRect tRect;
	getVisibleRect(pEntity, tRect);

	CMapRegionSearch tSearch;
	tSearch.searchEntity(this, tRect);

	// 玩家
	if (emEntityType_Player == pEntity->getEntityType())
	{
		// 通知周边玩家移除该玩家
		destroyPlayer2PlayerList(static_cast<CEntityPlayer*>(pEntity), tSearch.mPlayerList);
		// 通知该玩家移除周边玩家
		destroyPlayerList2Player(tSearch.mPlayerList, static_cast<CEntityPlayer*>(pEntity));
		// 通知该玩家移除周边NPC列表
		destroyNPCList2Player(tSearch.mNPCList, static_cast<CEntityPlayer*>(pEntity));
	}
	else
	{
		// 通知周边玩家移除该NPC
		destroyNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// 创建玩家
void CMap::createPlayer(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	addEntityToMapCell(pPlayer);
	onCreateEntityToMap(pPlayer);
	// 加入玩家列表中
	mPlayerList.push_back(pPlayer->getObjID());
	return;
}

/// 创建实体
CEntity* CMap::createEntity(CEntityCreator* pCreator)
{
	if (NULL == pCreator)
	{
		return NULL;
	}
	CEntity* pEntity = static_cast<CEntity*>(CEntity::createEntity(pCreator->getType()));
	if (NULL == pEntity)
	{
		return NULL;
	}
	pEntity->setTempID(pCreator->mTempID);
	pEntity->setLineID(pCreator->mLineID);
	pEntity->setMapID(pCreator->mMapID);
	pEntity->setMapIndex(pCreator->mMapIndex);
	pEntity->setPos(pCreator->mPos);
	pEntity->initEntity(pCreator);
	addEntityToMapCell(pEntity);
	onCreateEntityToMap(pEntity);
	// 加入实体列表中
	mEntityList.push_back(pEntity->getObjID());
	return pEntity;
}

/// 销毁玩家
void CMap::destroyPlayer(CEntityPlayer* pPlayer)
{
	removeEntityFromMap(pPlayer);

	PLAYER_LIST::iterator it = mPlayerList.begin();
	for (; it != mPlayerList.end(); ++it)
	{
		if (*it == pPlayer->getObjID())
		{
			mPlayerList.erase(it);
			break;
		}
	}
}

/// 销毁实体
void CMap::destroyEntity(CEntity* pEntity)
{
	removeEntityFromMap(pEntity);
	CEntity::destroyEntity(pEntity);
}


/// 销毁地图上所有的实体（不包含玩家）
void CMap::destroyAllEntity()
{
	ENTITY_LIST::iterator it = mEntityList.begin();
	for (; it != mEntityList.end(); )
	{
		CEntity* pEntity = static_cast<CEntity*>(CObjPool::Inst()->getObj(*it));
		if (NULL == pEntity)
		{
			continue;
		}
		// 先将迭代器移到下一个，就算destroyEntity中将其删除，也没关系
		++ it;
		destroyEntity(pEntity);
	}

	mEntityList.clear();
}


/// 通知其他玩家创建该玩家
void CMap::createPlayer2PlayerList(CEntityPlayer* pPlayer, std::vector<CEntityPlayer*>& rPlayerList)
{
	if (NULL == pPlayer)
	{
		return;
	}
	CCreatePlayerListNotify tCreatePlayerListNotify;
	PBPlayerSceneInfo* pPlayerInfo = tCreatePlayerListNotify.add_playerinfo();
	pPlayer->serializeSceneInfoToPB(pPlayerInfo);
	for (unsigned int i = 0; i < rPlayerList.size(); ++ i)
	{
		pPlayer->addVisiblePlayer(rPlayerList[i]);
		CSceneJob::Inst()->send2Player(rPlayerList[i], ID_S2C_NOTIYF_CREATE_PLAYER_LIST, &tCreatePlayerListNotify);
	}
}

/// 通知该玩家创建其他玩家
void CMap::createPlayerList2Player(std::vector<CEntityPlayer*>& rPlayerList, CEntityPlayer* pPlayer)
{
	CCreatePlayerListNotify tCreatePlayerListNotify;
	for (unsigned int i = 0; i < rPlayerList.size(); ++ i)
	{
		rPlayerList[i]->addVisiblePlayer(pPlayer);
		PBPlayerSceneInfo* pPlayerInfo = tCreatePlayerListNotify.add_playerinfo();
		rPlayerList[i]->serializeSceneInfoToPB(pPlayerInfo);
	}
	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIYF_CREATE_PLAYER_LIST, &tCreatePlayerListNotify);
}

/// 通知该玩家创建NPC列表
void CMap::createNPCList2Player(std::vector<CEntityNPC*>& rNPCList, CEntityPlayer* pPlayer)
{
	CCreateNPCListNotify tCreateNPCListNotify;
	for (unsigned int i = 0; i < rNPCList.size(); ++ i)
	{
		rNPCList[i]->addVisiblePlayer(pPlayer);
		PBNpcSceneInfo* pNpcSceneInfo = tCreateNPCListNotify.add_npcinfo();
		rNPCList[i]->serializeSceneInfoToPB(pNpcSceneInfo);
	}

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIYF_CREATE_NPC_LIST, &tCreateNPCListNotify);
}

/// 通知该玩家创建NPC
void CMap::createNPC2PlayerList(CEntityNPC* pNPC, std::vector<CEntityPlayer*>& rPlayerList)
{
	if (NULL == pNPC)
	{
		return;
	}
	CCreateNPCListNotify tCreateNPCListNotify;
	pNPC->serializeSceneInfoToPB(tCreateNPCListNotify.add_npcinfo());
	for (unsigned int i = 0; i < rPlayerList.size(); ++ i)
	{
		pNPC->addVisiblePlayer(rPlayerList[i]);
		CSceneJob::Inst()->send2Player(rPlayerList[i], ID_S2C_NOTIYF_CREATE_NPC_LIST, &tCreateNPCListNotify);
	}
}

/// 通知其他玩家销毁该玩家
void CMap::destroyPlayer2PlayerList(CEntityPlayer* pPlayer, std::vector<CEntityPlayer*>& rPlayerList)
{
	if (NULL == pPlayer)
	{
		return;
	}
	CDestroyEntityNotify tDestroyEntityNotify;
	tDestroyEntityNotify.add_entityid(pPlayer->getObjID());
	for (unsigned int i = 0; i < rPlayerList.size(); ++i)
	{
		pPlayer->removeVisiblePlayer(rPlayerList[i]);
		CSceneJob::Inst()->send2Player(rPlayerList[i], ID_S2C_NOTIYF_DESTROY_ENTITY, &tDestroyEntityNotify);
	}
}

/// 通知该玩家销毁其他玩家
void CMap::destroyPlayerList2Player(std::vector<CEntityPlayer*>& rPlayerList, CEntityPlayer* pPlayer)
{
	CDestroyEntityNotify tDestroyEntityNotify;
	for (unsigned int i = 0; i < rPlayerList.size(); ++i)
	{
		rPlayerList[i]->removeVisiblePlayer(pPlayer);
		tDestroyEntityNotify.add_entityid(rPlayerList[i]->getObjID());
	}

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIYF_DESTROY_ENTITY, &tDestroyEntityNotify);
}

/// 通知该玩家销毁NPC列表
void CMap::destroyNPCList2Player(std::vector<CEntityNPC*>& rNPCList, CEntityPlayer* pPlayer)
{
	CDestroyEntityNotify tDestroyEntityNotify;
	for (unsigned int i = 0; i < rNPCList.size(); ++ i)
	{
		rNPCList[i]->removeVisiblePlayer(pPlayer);
		tDestroyEntityNotify.add_entityid(rNPCList[i]->getObjID());
	}
	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIYF_DESTROY_ENTITY, &tDestroyEntityNotify);
}

/// 通知其他玩家销毁该NPC
void CMap::destroyNPC2PlayerList(CEntityNPC* pNPC, std::vector<CEntityPlayer*>& rPlayerList)
{
	if (NULL == pNPC)
	{
		return;
	}
	CDestroyEntityNotify tDestroyEntityNotify;
	tDestroyEntityNotify.add_entityid(pNPC->getObjID());
	for (unsigned int i = 0; i < rPlayerList.size(); ++i)
	{
		pNPC->removeVisiblePlayer(rPlayerList[i]);
		CSceneJob::Inst()->send2Player(rPlayerList[i], ID_S2C_NOTIYF_DESTROY_ENTITY, &tDestroyEntityNotify);
	}
}

void CMapManager::clear()
{
	MAP_LIST::iterator it = mMapList.begin();
	for (; it != mMapList.end(); ++ it)
	{
		delete it->second;
	}
	mMapList.clear();
}

CMap* CMapManager::createMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, short nLength, short nWidth)
{
	CMap* pNewMap = new CMap;
	if (NULL == pNewMap)
	{
		LOG_ERROR("new CMap failure, LineID: %d, MapID: %d, MapIndex: %d", nLineID, nMapID, nMapIndex);
		return NULL;
	}
	int nResult = pNewMap->initSize(nLength, nWidth);
	if (SUCCESS != nResult)
	{
		LOG_ERROR("new map unit failure, LineID: %d, MapID: %d, MapIndex: %d, Length: %d, Width: %d", nLineID, nMapID, nMapIndex, nLength, nWidth);
		delete pNewMap;
		return NULL;
	}
	bool bResult = insertMap(nLineID, nMapID, nMapIndex, pNewMap);
	if (!bResult)
	{
		LOG_ERROR("insert map into map list failure, LineID: %d, MapID: %d, MapIndex: %d", nLineID, nMapID, nMapIndex);
		delete pNewMap;
		return NULL;
	}
	return pNewMap;
}

CMap* CMapManager::getMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex)
{
	uint64 nKey = MAKE_MAP_KEY((uint64)nLineID, (uint64)nMapID, (uint64)nMapIndex);
	MAP_LIST::iterator it = mMapList.find(nKey);
	if (it != mMapList.end())
	{
		return it->second;
	}
	return NULL;
}

bool CMapManager::insertMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, CMap* pMap)
{
	if (NULL == pMap)
	{
		return false;
	}
	uint64 nKey = MAKE_MAP_KEY((uint64)nLineID, (uint64)nMapID, (uint64)nMapIndex);
	std::pair<MAP_LIST::iterator,bool> tResult = mMapList.insert(std::pair<uint64, CMap*>(nKey, pMap));
	return tResult.second;
}

/// 销毁地图
void CMapManager::destroyMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex)
{
	uint64 nKey = MAKE_MAP_KEY((uint64)nLineID, (uint64)nMapID, (uint64)nMapIndex);
	MAP_LIST::iterator it = mMapList.find(nKey);
	if (it == mMapList.end())
	{
		return;
	}
	CMap* pMap = it->second;
	if (NULL == pMap)
	{
		return;
	}
	// 销毁所有的实体
	pMap->destroyAllEntity();
	// 清除资源
	pMap->clear();
	CMapConfig* pMapConfig = CMapConfigManager::Inst()->getMapConfig(nMapID);
	if (NULL != pMapConfig && pMapConfig->getAutoDestory())
	{
		mTimerDestroyList.erase(nKey);
	}
	mMapList.erase(nKey);
	delete pMap;
}

void CMapManager::checkAutoDestoryMap()
{
	time_t tNowTime = CTimeManager::Inst()->getCurrTime();
	TIMER_DESTROY_SET::iterator it = mTimerDestroyList.begin();
	for (; it != mTimerDestroyList.end(); )
	{
		uint64 nKey = *it;
		MAP_LIST::iterator it = mMapList.find(nKey);
		if (mMapList.end() == it)
		{
			continue;
		}
		CMap* pMap = it->second;
		if (NULL == pMap)
		{
			continue;
		}

		++ it;
		if (pMap->getPlayerList().size() == 0)
		{
			time_t nNoPlayerTime = pMap->getNoPlayerTime();
			if (0 == nNoPlayerTime)
			{
				pMap->setNoPlayerTime(tNowTime);
				continue;
			}

			if (tNowTime - nNoPlayerTime > AUTO_DESTORY_MAP_TIME)
			{
				destroyMap(MAP_KEY_TO_LINE_ID(nKey), MAP_KEY_TO_MAP_ID(nKey), MAP_KEY_TO_MAP_INDEX(nKey));
			}
		}
		else
		{
			pMap->setNoPlayerTime(0);
		}
	}
}