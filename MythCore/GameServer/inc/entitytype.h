#ifndef __ENTITYTYPE_H__
#define __ENTITYTYPE_H__
enum EmEntityType
{
	emEntityType_None			= 0,	// 无
	emEntityType_Player			= 1,	// 玩家
	emEntityType_Ogre			= 2,	// 怪物
	emEntityType_FuncNPC		= 3,	// 功能NPC
	emEntityType_Item			= 4,	// 道具
};


enum EmAliveState
{
	emAliveState_Alive			= 0,	// 正常状态
	emAliveState_Dead			= 1,	// 死亡状态
};
#define MAX_PLAYER_TIMER_NUM	64		// 玩家最大的计时器数目
#define MAX_NPC_TIMER_NUM		16		// NPC最大的计时器数目
#define ENTITY_PRO_MASK			0x00FF	// 实体属性掩码
#define ENTITY_PRO_PER_START	0x0100	// 实体百分比属性开始（和EmProType对应）
#endif