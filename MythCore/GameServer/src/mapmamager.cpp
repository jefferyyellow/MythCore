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

CMapUnit::ENTITY_ALLOC CMapUnit::mEntityAlloc;
void CMapUnit::pushEntity(uint32 nObjID)
{
	CShareList<uint32>::CShareListNode<uint32>* pNode = mEntityAlloc.allocate();
	pNode->mData = nObjID;
	pNode->mpNext = NULL;
	pNode->mpPrev = NULL;
	mEntityList.push_front(pNode);
}

void CMapUnit::removeEntity(uint32 nObjID)
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

int CMap::init(short nLength, short nWidth)
{
	mMapUnit = new CMapUnit[nLength * nWidth];
	if (NULL == mMapUnit)
	{
		LOG_ERROR("create map unit failure!");
		return ERROR_MAP_CREATE_MAP_UNIT_FAILURE;
	}

	mLength = nLength;
	mWidth = nWidth;
	return SUCCESS;
}

void CMap::clear()
{
	if (NULL != mMapUnit)
	{
		delete[]mMapUnit;
	}
}

/// 得到可以区域
void CMap::getVisibleRect(CEntityCharacter* pEntity, CMythRect& rRect)
{
	rRect.mTopLeft.mX = (std::max)(pEntity->getPos().mX - VISIBLE_RADIUS_X, 0);
	rRect.mTopLeft.mY = (std::max)(pEntity->getPos().mY - VISIBLE_RADIUS_Y, 0);

	rRect.mBottomRight.mX = (std::min)(pEntity->getPos().mX + VISIBLE_RADIUS_X, getLength() - 1);
	rRect.mBottomRight.mY = (std::min)(pEntity->getPos().mY + VISIBLE_RADIUS_Y, getWidth() - 1);
	return;
}

/// 实体移动
void CMap::onEntityMove(CEntityCharacter* pEntity, CMythPoint& rDesPos)
{
	MYTH_ASSERT(NULL == pEntity, return);
	// 已经在目的地了
	if (pEntity->getPos() == rDesPos)
	{
		return;
	}

	CMythRect tSrcRect;
	getVisibleRect(pEntity, tSrcRect);
	
	// 从原来的地图单元里移除
	if (!removeEntityFromMapUnit(pEntity))
	{
		return;
	}

	pEntity->setPos(rDesPos);
	CMythRect tDesRect;
	getVisibleRect(pEntity, tDesRect);
	// 从原来的地图单元移除触发
	onRemoveEntityFromMapUnit(pEntity, tSrcRect, tDesRect);
	// 加入到新的地图单元
	addEntityToMapUnit(pEntity);
	// 加入到新的地图单元触发
	onAddEntityToMapUnit(pEntity, tSrcRect, tDesRect);
}


/// 把实体加入地图单元
bool CMap::addEntityToMapUnit(CEntity* pEntity)
{
	MYTH_ASSERT(NULL == pEntity, return false);

	CMapUnit* pMapUnit = getMapUnit(pEntity->getPosX(), pEntity->getPosY());
	if (NULL == pMapUnit)
	{
		return false;
	}

	pMapUnit->pushEntity(pEntity->getObjID());
	return true;
}

/// 把实体加入地图单元触发
void CMap::onAddEntityToMapUnit(CEntityCharacter* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect)
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
		createPlayerList2Player(tSearch.mPlayerList, (CEntityPlayer*)pEntity);
		// 告诉该玩家创建周边玩家
		createPlayer2PlayerList((CEntityPlayer*)pEntity, tSearch.mPlayerList);
		// 告诉该玩家创建NPC
		createNPCList2Player(tSearch.mNPCList, (CEntityPlayer*)pEntity);
	}
	else
	{
		// 告诉周边的玩家创建该NPC
		createNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// 将实体从地图单元中移除
bool CMap::removeEntityFromMapUnit(CEntity* pEntity)
{
	MYTH_ASSERT(NULL == pEntity, return false);

	CMapUnit* pMapUnit = getMapUnit(pEntity->getPosX(), pEntity->getPosY());
	if (NULL == pMapUnit)
	{
		return false;
	}
	pMapUnit->removeEntity(pEntity->getObjID());
	return true;
}

/// 将实体从地图单元中移除触发
void CMap::onRemoveEntityFromMapUnit(CEntityCharacter* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect)
{
	MYTH_ASSERT(NULL == pEntity, return);
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
		destroyPlayer2PlayerList((CEntityPlayer*)pEntity, tSearch.mPlayerList);
		// 通知该玩家移除周边玩家
		destroyPlayerList2Player(tSearch.mPlayerList, (CEntityPlayer*)pEntity);
		// 通知该玩家移除周边NPC列表
		destroyNPCList2Player(tSearch.mNPCList, (CEntityPlayer*)pEntity);
	}
	else
	{
		// 通知周边玩家移除该NPC
		destroyNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}


/// 在地图中创建实体触发
void CMap::onCreateEntityToMapUnit(CEntityCharacter* pEntity)
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
		createPlayerList2Player(tSearch.mPlayerList, (CEntityPlayer*)pEntity);
		// 告诉该玩家创建周边玩家
		createPlayer2PlayerList((CEntityPlayer*)pEntity, tSearch.mPlayerList);
		// 告诉该玩家创建NPC
		createNPCList2Player(tSearch.mNPCList, (CEntityPlayer*)pEntity);
	}
	else
	{
		// 告诉周边的玩家创建该NPC
		createNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// 在地图中移除实体触发
void CMap::onRemoveEntityToMapUnit(CEntityCharacter* pEntity)
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
		destroyPlayer2PlayerList((CEntityPlayer*)pEntity, tSearch.mPlayerList);
		// 通知该玩家移除周边玩家
		destroyPlayerList2Player(tSearch.mPlayerList, (CEntityPlayer*)pEntity);
		// 通知该玩家移除周边NPC列表
		destroyNPCList2Player(tSearch.mNPCList, (CEntityPlayer*)pEntity);
	}
	else
	{
		// 通知周边玩家移除该NPC
		destroyNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}


/// 创建NPC
CEntityNPC* CMap::createNPC(int nNPCID, CMythPoint& rPos)
{
	CTplNPC* pTplNPC = (CTplNPC*)CStaticData::SearchTpl(nNPCID);
	if (NULL == pTplNPC)
	{
		return NULL;
	}
	EmEntityType eEntityType = emEntityType_None;
	if (emTemplateType_FuncNPC == pTplNPC->mTemplateType)
	{
		eEntityType = emEntityType_FuncNPC;
	}
	else
	{
		eEntityType = emEntityType_Ogre;
	}


	CEntityNPC* pEntityNPC = (CEntityNPC*)CEntity::createEntity(eEntityType);
	if (NULL == pEntityNPC)
	{
		return NULL;
	}
	pEntityNPC->setTempID(nNPCID);
	pEntityNPC->setPos(rPos);
	addEntityToMapUnit(pEntityNPC);
	onCreateEntityToMapUnit(pEntityNPC);
	return pEntityNPC;
}

/// 创建玩家
CEntityPlayer* CMap::createPlayer(CEntityPlayer* pPlayer)
{
	return NULL;
}

/// 通知其他玩家创建该玩家
void CMap::createPlayer2PlayerList(CEntityPlayer* pPlayer, std::vector<CEntityPlayer*>& rPlayerList)
{
	if (NULL == pPlayer)
	{
		return;
	}
	CMessageCreatePlayerListNotify tCreatePlayerListNotify;
	PBPlayerSceneInfo* pPlayerInfo = tCreatePlayerListNotify.add_playerinfo();
	pPlayer->serializeSceneInfoToPB(pPlayerInfo);
	for (unsigned int i = 0; i < rPlayerList.size(); ++ i)
	{
		pPlayer->addVisiblePlayer(rPlayerList[i]);
		CSceneJob::Inst()->sendClientMessage(rPlayerList[i], ID_S2C_NOTIYF_CREATE_PLAYER_LIST, &tCreatePlayerListNotify);
	}
}

/// 通知该玩家创建其他玩家
void CMap::createPlayerList2Player(std::vector<CEntityPlayer*>& rPlayerList, CEntityPlayer* pPlayer)
{
	CMessageCreatePlayerListNotify tCreatePlayerListNotify;
	for (unsigned int i = 0; i < rPlayerList.size(); ++ i)
	{
		rPlayerList[i]->addVisiblePlayer(pPlayer);
		PBPlayerSceneInfo* pPlayerInfo = tCreatePlayerListNotify.add_playerinfo();
		rPlayerList[i]->serializeSceneInfoToPB(pPlayerInfo);
	}
	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_CREATE_PLAYER_LIST, &tCreatePlayerListNotify);
}

/// 通知该玩家创建NPC列表
void CMap::createNPCList2Player(std::vector<CEntityNPC*>& rNPCList, CEntityPlayer* pPlayer)
{
	CMessageCreateNPCListNotify tCreateNPCListNotify;
	for (unsigned int i = 0; i < rNPCList.size(); ++ i)
	{
		rNPCList[i]->addVisiblePlayer(pPlayer);
		PBNpcSceneInfo* pNpcSceneInfo = tCreateNPCListNotify.add_npcinfo();
		rNPCList[i]->serializeSceneInfoToPB(pNpcSceneInfo);
	}

	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_CREATE_NPC_LIST, &tCreateNPCListNotify);
}

/// 通知该玩家创建NPC
void CMap::createNPC2PlayerList(CEntityNPC* pNPC, std::vector<CEntityPlayer*>& rPlayerList)
{
	if (NULL == pNPC)
	{
		return;
	}
	CMessageCreateNPCListNotify tCreateNPCListNotify;
	pNPC->serializeSceneInfoToPB(tCreateNPCListNotify.add_npcinfo());
	for (unsigned int i = 0; i < rPlayerList.size(); ++ i)
	{
		pNPC->addVisiblePlayer(rPlayerList[i]);
		CSceneJob::Inst()->sendClientMessage(rPlayerList[i], ID_S2C_NOTIYF_CREATE_NPC_LIST, &tCreateNPCListNotify);
	}
}

/// 通知其他玩家销毁该玩家
void CMap::destroyPlayer2PlayerList(CEntityPlayer* pPlayer, std::vector<CEntityPlayer*>& rPlayerList)
{
	if (NULL == pPlayer)
	{
		return;
	}
	CMessageDestroyEntityNotify tDestroyEntityNotify;
	tDestroyEntityNotify.add_entityid(pPlayer->getObjID());
	for (unsigned int i = 0; i < rPlayerList.size(); ++i)
	{
		pPlayer->removeVisiblePlayer(rPlayerList[i]);
		CSceneJob::Inst()->sendClientMessage(rPlayerList[i], ID_S2C_NOTIYF_DESTROY_ENTITY, &tDestroyEntityNotify);
	}
}

/// 通知该玩家销毁其他玩家
void CMap::destroyPlayerList2Player(std::vector<CEntityPlayer*>& rPlayerList, CEntityPlayer* pPlayer)
{
	CMessageDestroyEntityNotify tDestroyEntityNotify;
	for (unsigned int i = 0; i < rPlayerList.size(); ++i)
	{
		rPlayerList[i]->removeVisiblePlayer(pPlayer);
		tDestroyEntityNotify.add_entityid(rPlayerList[i]->getObjID());
	}

	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_DESTROY_ENTITY, &tDestroyEntityNotify);
}

/// 通知该玩家销毁NPC列表
void CMap::destroyNPCList2Player(std::vector<CEntityNPC*>& rNPCList, CEntityPlayer* pPlayer)
{
	CMessageDestroyEntityNotify tDestroyEntityNotify;
	for (unsigned int i = 0; i < rNPCList.size(); ++ i)
	{
		rNPCList[i]->removeVisiblePlayer(pPlayer);
		tDestroyEntityNotify.add_entityid(rNPCList[i]->getObjID());
	}
	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_DESTROY_ENTITY, &tDestroyEntityNotify);
}

/// 通知其他玩家销毁该NPC
void CMap::destroyNPC2PlayerList(CEntityNPC* pNPC, std::vector<CEntityPlayer*>& rPlayerList)
{
	if (NULL == pNPC)
	{
		return;
	}
	CMessageDestroyEntityNotify tDestroyEntityNotify;
	tDestroyEntityNotify.add_entityid(pNPC->getObjID());
	for (unsigned int i = 0; i < rPlayerList.size(); ++i)
	{
		pNPC->removeVisiblePlayer(rPlayerList[i]);
		CSceneJob::Inst()->sendClientMessage(rPlayerList[i], ID_S2C_NOTIYF_DESTROY_ENTITY, &tDestroyEntityNotify);
	}
}


CMap* CMapManager::createMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, short nLength, short nWidth)
{
	CMap* pNewMap = new CMap;
	if (NULL == pNewMap)
	{
		LOG_ERROR("new CMap failure, LineID: %d, MapID: %d, MapIndex: %d", nLineID, nMapID, nMapIndex);
		return NULL;
	}
	int nResult = pNewMap->init(nLength, nWidth);
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
