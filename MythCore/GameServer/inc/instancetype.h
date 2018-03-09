#ifndef __INSTANCETYPE_H__
#define __INSTANCETYPE_H__

#define MAX_INSTANCE_MAP_NUM 10				// 副本最大地图数

enum EmInstanceStatus
{
	emInstanceStatus_None	= 0,			// 无
	emInstanceStatus_Init	= 1,			// 初始化
	emInstanceStatus_Play	= 2,			// 开始玩
	emInstanceStatus_End	= 3,			// 结束
	emInstanceStatus_Destory= 4,			// 销毁
};
#endif