#ifndef __SERVERACTIVITYTYPE_H__
#define __SERVERACTIVITYTYPE_H__

#define ACTIVITY_PRIZE_NUM		4	// 活动奖励数目
#define PHASE_ACTIVITY_NUM		30	// 阶段条件活动数据

enum EmServerActState
{
	emServerActState_End		= 0,		// 结束状态
	emServerActState_Start		= 1,		// 开始状态
	emServerActState_Prize		= 2,		// 奖励状态
};


enum EmServerActType
{
	emServerActType_None		= 0,		// 无类型
	emServerActType_Phase		= 1,		// 条件阶段获得
};

#endif