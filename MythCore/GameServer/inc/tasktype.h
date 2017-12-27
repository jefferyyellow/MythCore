#ifndef __TASKTYPE_H__
#define __TASKTYPE_H__

#define		MAX_PLAYER_TASK_NUM			20			// 最多的可接任务数目
#define		MAX_PLAYER_TASK_PARAM		2			// 最多的任务参数数目

#define MAX_ACCEPT_CONDITION_PARAM		4			// 最大的接受条件参数
#define MAX_COMPLETE_CONDITION_PARAM	4			// 最大的完成条件参数
#define MAX_TASK_REWARD_PARAM			4			// 最大的任务奖励参数
#define MAX_TASK_INVALID_PARAM			2			// 最大的任务失效参数

#define	MAX_ACCEPT_CONDITION			4			// 最大的任务条件
#define MAX_COMPLETE_CONDITION			4			// 最大的完成条件
#define MAX_CLOSE_CONDITION				4			// 最大的失效条件
#define MAX_TASK_PRIZE_NUM				8			// 最大的奖励数目
#define MAX_TASK_ID						8192		// 所有任务最大ID



enum EmAcceptCondition
{
	emAccept_None						= 0,		// 无
	emAccept_PreTask					= 1,		// 前置任务				参数0，前置任务ID
	emAccept_Level						= 2,		// 等级					参数0，最小等级
};

enum EmCompleteCondition
{
	emComplete_None						= 0,		// 无
	emComplete_KillOgre					= 1,		// 杀怪计数(通知前端)	参数0：怪物ID，		参数1：怪物数目
	emComplete_HoldItem					= 2,		// 拥有道具				参数0：道具ID，		参数1：道具数目
	emComplete_Level					= 3,		// 等级					参数0：玩家等级		
	emComplete_PassFB					= 4,		// 通关副本(通知前端)	参数0：地图ID		参数1：副本类型			参数2：副本索引		参数3：通过次数
	emComplete_Collect					= 5,		// 采集(通知前端)		参数0：道具ID		参数1：道具数目
	emComplete_Talk						= 6,		// 对话
};

enum EmCloseCondition
{
	emClose_None						= 0,		// 无
	emClose_Prestige					= 1,		// 声望
	emClose_Level						= 2,		// 等级
	emClose_PreTask						= 3,		// 前置任务
};

enum EmTaskPrizeType
{
	emTaskPrize_None					= 0,		// 无
	emTaskPrize_Item					= 1,		// 道具(货币）
};

#endif