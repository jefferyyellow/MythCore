#ifndef __MAPTYPE_H__
#define __MAPTYPE_H__

#define VISIBLE_RADIUS_X 10				// 可见区域X方向半径
#define VISIBLE_RADIUS_Y 10				// 可见区域Y方向半径
enum EmMapType
{
	emMapType_None			= 0,		// 无类型
	emMapType_City			= 1,		// 城镇
	emMapType_Field			= 2,		// 野外
	emMapType_Instance		= 3,		// 副本
};

#define MAX_MAP_ID			256			// 最大的地图ID	
#define MAX_MAP_PORTAL_NUM	8			// 地图中最大的传送门的数目
#define MAP_UNIT_SIZE		40			// 地图单元大小
#endif