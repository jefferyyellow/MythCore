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

/// �õ���������
void CMap::getVisibleRect(CEntity* pEntity, CMythRect& rRect)
{
	rRect.mTopLeft.mX = (std::max)(pEntity->getPos().mX - VISIBLE_RADIUS_X, 0);
	rRect.mTopLeft.mY = (std::max)(pEntity->getPos().mY - VISIBLE_RADIUS_Y, 0);

	rRect.mBottomRight.mX = (std::min)(pEntity->getPos().mX + VISIBLE_RADIUS_X, getLength() - 1);
	rRect.mBottomRight.mY = (std::min)(pEntity->getPos().mY + VISIBLE_RADIUS_Y, getWidth() - 1);
	return;
}

/// ʵ���ƶ�
void CMap::onEntityMove(CEntity* pEntity, CMythPoint& rDesPos)
{
	MYTH_ASSERT(NULL != pEntity, return);
	// �Ѿ���Ŀ�ĵ���
	if (pEntity->getPos() == rDesPos)
	{
		return;
	}

	CMythRect tSrcRect;
	getVisibleRect(pEntity, tSrcRect);
	
	// ��ԭ���ĵ�ͼ��Ԫ���Ƴ�
	if (!removeEntityFromMapCell(pEntity))
	{
		return;
	}

	pEntity->setPos(rDesPos);
	CMythRect tDesRect;
	getVisibleRect(pEntity, tDesRect);
	// ��ԭ���ĵ�ͼ��Ԫ�Ƴ�����
	onRemoveEntityFromMapCell(pEntity, tSrcRect, tDesRect);
	// ���뵽�µĵ�ͼ��Ԫ
	addEntityToMapCell(pEntity);
	// ���뵽�µĵ�ͼ��Ԫ����
	onAddEntityToMapCell(pEntity, tSrcRect, tDesRect);
}


/// ��ʵ������ͼ��Ԫ
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

/// ��ʵ������ͼ��Ԫ����
void CMap::onAddEntityToMapCell(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect)
{
	CMythRect tRectArray[4];
	// tDesRect - tSrcRect,����������һ����������
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

	// ���
	if (emEntityType_Player == pEntity->getEntityType())
	{
		// �����ܱߵ���Ҵ��������
		createPlayerList2Player(tSearch.mPlayerList, static_cast<CEntityPlayer*>(pEntity));
		// ���߸���Ҵ����ܱ����
		createPlayer2PlayerList(static_cast<CEntityPlayer*>(pEntity), tSearch.mPlayerList);
		// ���߸���Ҵ���NPC
		createNPCList2Player(tSearch.mNPCList, static_cast<CEntityPlayer*>(pEntity));
	}
	else
	{
		// �����ܱߵ���Ҵ�����NPC
		createNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// ��ʵ��ӵ�ͼ��Ԫ���Ƴ�
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

/// ��ʵ��ӵ�ͼ��Ԫ���Ƴ�����
void CMap::onRemoveEntityFromMapCell(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect)
{
	MYTH_ASSERT(NULL != pEntity, return);
	CMythRect tRectArray[4];
	// tSrcRect - tDesRect,��ԭ������һ������ɾ��
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

	// ���
	if (emEntityType_Player == pEntity->getEntityType())
	{
		// ֪ͨ�ܱ�����Ƴ������
		destroyPlayer2PlayerList(static_cast<CEntityPlayer*>(pEntity), tSearch.mPlayerList);
		// ֪ͨ������Ƴ��ܱ����
		destroyPlayerList2Player(tSearch.mPlayerList, static_cast<CEntityPlayer*>(pEntity));
		// ֪ͨ������Ƴ��ܱ�NPC�б�
		destroyNPCList2Player(tSearch.mNPCList, static_cast<CEntityPlayer*>(pEntity));
	}
	else
	{
		// ֪ͨ�ܱ�����Ƴ���NPC
		destroyNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// �ڵ�ͼ�д���ʵ��
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

/// �ڵ�ͼ�д���ʵ�崥��
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

	// ���
	if (emEntityType_Player == pEntity->getEntityType())
	{
		// �����ܱߵ���Ҵ��������
		createPlayerList2Player(tSearch.mPlayerList, static_cast<CEntityPlayer*>(pEntity));
		// ���߸���Ҵ����ܱ����
		createPlayer2PlayerList(static_cast<CEntityPlayer*>(pEntity), tSearch.mPlayerList);
		// ���߸���Ҵ���NPC
		createNPCList2Player(tSearch.mNPCList, static_cast<CEntityPlayer*>(pEntity));
	}
	else
	{
		// �����ܱߵ���Ҵ�����NPC
		createNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// �ڵ�ͼ���Ƴ�ʵ��
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


/// �ڵ�ͼ���Ƴ�ʵ�崥��
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

	// ���
	if (emEntityType_Player == pEntity->getEntityType())
	{
		// ֪ͨ�ܱ�����Ƴ������
		destroyPlayer2PlayerList(static_cast<CEntityPlayer*>(pEntity), tSearch.mPlayerList);
		// ֪ͨ������Ƴ��ܱ����
		destroyPlayerList2Player(tSearch.mPlayerList, static_cast<CEntityPlayer*>(pEntity));
		// ֪ͨ������Ƴ��ܱ�NPC�б�
		destroyNPCList2Player(tSearch.mNPCList, static_cast<CEntityPlayer*>(pEntity));
	}
	else
	{
		// ֪ͨ�ܱ�����Ƴ���NPC
		destroyNPC2PlayerList((CEntityNPC*)pEntity, tSearch.mPlayerList);
	}
}

/// �������
void CMap::createPlayer(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	addEntityToMapCell(pPlayer);
	onCreateEntityToMap(pPlayer);
	// ��������б���
	mPlayerList.push_back(pPlayer->getObjID());
	return;
}

/// ����ʵ��
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
	// ����ʵ���б���
	mEntityList.push_back(pEntity->getObjID());
	return pEntity;
}

/// �������
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

/// ����ʵ��
void CMap::destroyEntity(CEntity* pEntity)
{
	removeEntityFromMap(pEntity);
	CEntity::destroyEntity(pEntity);
}


/// ���ٵ�ͼ�����е�ʵ�壨��������ң�
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
		// �Ƚ��������Ƶ���һ��������destroyEntity�н���ɾ����Ҳû��ϵ
		++ it;
		destroyEntity(pEntity);
	}

	mEntityList.clear();
}


/// ֪ͨ������Ҵ��������
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

/// ֪ͨ����Ҵ����������
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

/// ֪ͨ����Ҵ���NPC�б�
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

/// ֪ͨ����Ҵ���NPC
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

/// ֪ͨ����������ٸ����
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

/// ֪ͨ����������������
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

/// ֪ͨ���������NPC�б�
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

/// ֪ͨ����������ٸ�NPC
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

/// ���ٵ�ͼ
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
	// �������е�ʵ��
	pMap->destroyAllEntity();
	// �����Դ
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