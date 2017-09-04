#include "mapmamager.h"
#include "errcode.h"
#include "locallogjob.h"
#include "entity.h"
#include "mapregionsearch.h"

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
		// 告诉该玩家创建周边玩家
		// 告诉该玩家创建NPC

	}
	else
	{
		// 告诉周边的玩家创建该NPC
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

	// 玩家
	if (emEntityType_Player == pEntity->getEntityType())
	{

	}
	else
	{

	}
}

/// 通知其他玩家创建该玩家
void CMap::createPlayer2PlayerList(CEntityPlayer* pPlayer, std::vector<CEntityPlayer*>& rPlayerList)
{

}

/// 通知该玩家创建其他玩家
void CMap::createPlayerList2Player()
{

}

/// 通知该玩家创建NPC列表
void CMap::createNPCList2Player()
{

}

int CMapManager::createMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, short nLength, short nWidth)
{
	CMap* pNewMap = new CMap;
	if (NULL == pNewMap)
	{
		LOG_ERROR("new CMap failure, LineID: %d, MapID: %d, MapIndex: %d", nLineID, nMapID, nMapIndex);
		return ERROR_MAP_CREATE_MAP_MEMORY;
	}
	int nResult = pNewMap->init(nLength, nWidth);
	if (SUCCESS != nResult)
	{
		LOG_ERROR("new map unit failure, LineID: %d, MapID: %d, MapIndex: %d, Length: %d, Width: %d", nLineID, nMapID, nMapIndex, nLength, nWidth);
		delete pNewMap;
		return nResult;
	}
	bool bResult = insertMap(nLineID, nMapID, nMapIndex, pNewMap);
	if (!bResult)
	{
		LOG_ERROR("insert map into map list failure, LineID: %d, MapID: %d, MapIndex: %d", nLineID, nMapID, nMapIndex);
		delete pNewMap;
		return ERROR_MAP_ADD_MAP_LIST_FAILURE;
	}
	return SUCCESS;
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
