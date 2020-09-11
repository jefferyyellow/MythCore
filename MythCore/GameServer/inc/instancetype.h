#ifndef __INSTANCETYPE_H__
#define __INSTANCETYPE_H__

#define MAX_INSTANCE_MAP_NUM	8				// 副本最大地图数
// 普通副本500 爬塔副本 500
// 其他类型的副本 200，总共可以分成 7类了
#define MAX_INSTANCE_ID			2000			// 最大的副本ID

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
	emInstance_Team			= 2,			// 普通组队副本
};

enum EmInstanceEvent
{
	emInstEvent_None			= 0,			// 无
	emInstEvent_OgreDead		= 1,			// 怪物死亡
	emInstEvent_PlayerDead		= 2,			// 玩家死亡
	emInstEvent_PlayerHP		= 3,			// 玩家血量变化
	emInstEventMax
};
#endif