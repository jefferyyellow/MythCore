#ifndef __INSTANCETYPE_H__
#define __INSTANCETYPE_H__

#define MAX_INSTANCE_MAP_NUM 4				// 副本最大地图数

enum EmInstanceStatus
{
	emInstanceStatus_None	= 0,			// 无
	emInstanceStatus_Init	= 1,			// 初始化
	emInstanceStatus_Play	= 2,			// 开始玩
	emInstanceStatus_End	= 3,			// 结束
	emInstanceStatus_Destory= 4,			// 销毁
};

enum EmInstanceType
{
	emInstance_None			= 0,			// 无
	// 单人副本部分
	emInstance_Common		= 1,			// 普通单人副本
	// 多人副本部分
	emInstance_Team			= 32,			// 普通组队副本
};


#endif