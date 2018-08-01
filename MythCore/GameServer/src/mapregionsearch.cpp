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
			CMapCell* pMapCell = pMap->getMapCell(nX, nY);
			searchMapCell(pMapCell);
		}
	}
}

// 搜索地图单元
void CMapRegionSearch::searchMapCell(CMapCell* pMapCell)
{
	if (NULL == pMapCell)
	{
		return;
	}

	CMapCell::ENTITY_LIST rEntityList = pMapCell->getEntityList();
	CMapCell::ENTITY_LIST::iterator it = rEntityList.begin();
	for (; it != rEntityList.end(); ++it)
	{
		CEntity* pEntity = static_cast<CEntity*>(CObjPool::Inst()->getObj(*it));
		if (NULL == pEntity)
		{
			continue;
		}
		if (emEntityType_Player == pEntity->getEntityType())
		{
			mPlayerList.push_back(static_cast<CEntityPlayer*>(pEntity));
		}
		else
		{
			mNPCList.push_back(static_cast<CEntityNPC*>(pEntity));
		}
	}
}