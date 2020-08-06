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


enum EmSvrActType
{
	emSvrActType_None			= 0,		// 无类型
	emSvrActType_CumRecharge	= 1,		// 累计充值
	emSvrActType_CumConsume		= 2,		// 累计消费
	emSvrActType_Rank			= 3,		// 排行活动
	emServerActTypeMax
};

#endif