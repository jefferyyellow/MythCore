#ifndef __DAILYACTIVITYTYPE_H__
#define __DAILYACTIVITYTYPE_H__

enum EmDailyActID
{
	emDailyActID_None = 0,
	emDailyActID_XXX = 1,
	emDailyActIDMax
};

enum EmDailyActStatus
{
	emDailyActStatus_None = 0,	// 无状态
	emDailyActStatus_Start = 1,	// 开始状态
	emDailyActStatus_End = 2,	// 结束状态
	emDailyActStatus_Notice = 3,	// 通知状态
};

#endif