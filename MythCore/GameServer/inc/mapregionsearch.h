#ifndef __MAPREGIONSEARCH_H__
#define __MAPREGIONSEARCH_H__
#include "geometrytype.h"
#include <vector>
using namespace std;
class CEntity;
class CEntityPlayer;
class CMapUnit;
class CMap;
class CMapRegionSearch
{
public:
	CMapRegionSearch();
	~CMapRegionSearch();
	void clear()
	{
		mPlayerList.clear();
		mEntityList.clear();
	}

public:
	/// 搜索实体
	void	searchEntity(CMap* pMap, CMythRect& tRect);

private:
	/// 搜索地图单元
	void	searchMapUnity(CMapUnit* pMapUnit);

public:
	std::vector<CEntityPlayer*>		mPlayerList;
	std::vector<CEntity*>			mEntityList;
};
#endif