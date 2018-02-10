#ifndef __MAPREGIONSEARCH_H__
#define __MAPREGIONSEARCH_H__
#include "geometrytype.h"
#include "servercommon.h"
using namespace std;
class CEntityNPC;
class CEntityPlayer;
class CMapUnit;
class CMap;
class CMapRegionSearch
{
public:
	CMapRegionSearch()
	{
	}
	~CMapRegionSearch()
	{
	}
	void clear()
	{
		mPlayerList.clear();
		mNPCList.clear();
	}

public:
	/// ����ʵ��
	void	searchEntity(CMap* pMap, CMythRect& tRect);

private:
	/// ������ͼ��Ԫ
	void	searchMapUnity(CMapUnit* pMapUnit);

public:
	std::vector<CEntityPlayer*>		mPlayerList;
	std::vector<CEntityNPC*>		mNPCList;
};
#endif