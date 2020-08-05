#ifndef __SERVERACTIVITYTYPE_H__
#define __SERVERACTIVITYTYPE_H__


#define MAKE_ACT_UNIQUE_ID(nType, nSubType, ID) (nType << 24 | nSubType << 16 | ID)
#define MAX_SERVER_ACT_NUM	128
enum EmServerActState
{
	emServerActState_End		= 0,		// 结束状态
	emServerActState_Start		= 1,		// 开始状态
	emServerActState_Prize		= 2,		// 奖励状态
};


enum EmServerActType
{
	emServerActType_None		= 0,		// 无类型
	emServerActTypeMax
};

#endif