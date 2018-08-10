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

#define MAX_PLAYER_TIMER_NUM	64		// 玩家最大的计时器数目
#define MAX_NPC_TIMER_NUM		16		// NPC最大的计时器数目
#endif