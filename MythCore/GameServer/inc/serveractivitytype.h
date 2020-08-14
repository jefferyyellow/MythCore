#ifndef __SERVERACTIVITYTYPE_H__
#define __SERVERACTIVITYTYPE_H__


#define MAKE_ACT_UNIQUE_ID(nType, nSubType, ID) (nType << 24 | nSubType << 16 | ID)
#define MAX_SERVER_ACT_NUM	128
enum EmSvrActState
{
	emSvrActState_End		= 0,		// 结束状态
	emSvrActState_Start		= 1,		// 开始状态
	emSvrActState_Prize		= 2,		// 奖励状态
};

// 运营活动
enum EmSvrActType
{
	emSvrActType_None			= 0,		// 无类型
	emSvrActType_CumRecharge	= 1,		// 累计充值
	emSvrActType_CumConsume		= 2,		// 累计消费
	emSvrActType_Rank			= 3,		// 排行活动
	emServerActTypeMax
};

// 排行榜活动子类型
enum EmRankActSubType
{
	emRankActSubType_None		= 0,		// 无类型
	emRankActSubType_Level		= 1,		// 等级排行榜活动
	emRankActSubType_Power		= 2,		// 战力排行榜活动
};

/// 活动函数类型
enum EmFuncType
{
	emFuncType_None				= 0,		// 无类型
	emFuncType_End				= 1,		// 活动结束函数
	emFuncType_ClearPlayerData	= 2,		// 清理玩家数据函数
	emFuncType_RefreshProcess	= 3,		// 刷新活动进度
};
#endif