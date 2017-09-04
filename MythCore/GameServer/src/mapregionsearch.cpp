#include "mapregionsearch.h"
#include "mapmamager.h"
#include "objpool.h"
#include "entitytype.h"
#include "entity.h"

// 搜索实体
void CMapRegionSearch::searchEntity(CMap* pMap, CMythRect& tRect)
{
	for (int nY = tRect.mTopLeft.mY; nY <= tRect.mBottomRight.mY; ++ nY)
	{
		for (int nX = tRect.mTopLeft.mX; nX <= tRect.mBottomRight.mX; ++nX)
		{
			CMapUnit* pMapUnit = pMap->getMapUnit(nX, nY);
			searchMapUnity(pMapUnit);
		}
	}
}

// 搜索地图单元
void CMapRegionSearch::searchMapUnity(CMapUnit* pMapUnit)
{
	if (NULL == pMapUnit)
	{
		return;
	}

	CMapUnit::ENTITY_LIST rEntityList = pMapUnit->GetEntityList();
	CMapUnit::ENTITY_LIST::iterator it = rEntityList.begin();
	for (; it != rEntityList.end(); ++it)
	{
		CEntity* pEntity = (CEntity*)CObjPool::Inst()->getObj(*it);
		if (NULL == pEntity)
		{
			continue;
		}
		if (emEntityType_Player == pEntity->getEntityType())
		{
			mPlayerList.push_back((CEntityPlayer*)pEntity);
		}
		else
		{
			mEntityList.push_back(pEntity);
		}
	}
}