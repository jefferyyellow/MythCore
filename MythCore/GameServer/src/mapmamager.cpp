#include "mapmamager.h"

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
			CShareList<uint32>::CShareListNode<uint32>* pNode = it.mNode;
			mEntityList.eraseNode(pNode);
			mEntityAlloc.free(pNode);
			break;
		}
	}
}

bool CMap::init(short nLength, short nWidth)
{
	mMapUnit = new CMapUnit[nLength * nWidth];
	if (NULL == mMapUnit)
	{
		return false;
	}

	mLength = nLength;
	mWidth = nWidth;
	return true;
}


CMap* CMapManager::getMap(short nLineID, short nMapID, int nMapIndex)
{
	uint64 nKey = MAKE_MAP_KEY((uint64)nLineID, (uint64)nMapID, (uint64)nMapIndex);
	MAP_LIST::iterator it = mMapList.find(nKey);
	if (it != mMapList.end())
	{
		return it->second;
	}

	return NULL;
}


bool CMapManager::insertMap(short nLineID, short nMapID, int nMapIndex, CMap* pMap)
{
	if (NULL == pMap)
	{
		return false;
	}
	uint64 nKey = MAKE_MAP_KEY((uint64)nLineID, (uint64)nMapID, (uint64)nMapIndex);
	std::pair<MAP_LIST::iterator,bool> tResult = mMapList.insert(std::pair<uint64, CMap*>(nKey, pMap));
	return tResult.second;
}
