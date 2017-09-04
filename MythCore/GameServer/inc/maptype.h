#ifndef __MAPTYPE_H__
#define __MAPTYPE_H__

#define VISIBLE_RADIUS_X 10				// 可见区域X方向半径
#define VISIBLE_RADIUS_Y 10				// 可见区域Y方向半径
enum EmMapType
{
	emMapType_None			= 0,	// 无类型
	emMapType_City			= 1,	// 城镇
	emMapType_Field			= 2,	// 野外
	emMapType_Instance		= 3,	// 副本
};


#endif