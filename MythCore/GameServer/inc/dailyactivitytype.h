#ifndef __DAILYACTIVITYTYPE_H__
#define __DAILYACTIVITYTYPE_H__

#define MAX_DAILY_ACT_ID	64
enum EmDailyActType
{
	emDailyActType_None			= 0,
	emDailyActType_XXX			= 1,
	emDailyActType_Time			= 2,
	emDailyActTypeMax
};

enum EmDailyActStatus
{
	emDailyActStatus_None		= 0,	// 无状态
	emDailyActStatus_Start		= 1,	// 开始状态
	emDailyActStatus_End		= 2,	// 结束状态
	emDailyActStatus_Notice		= 3,	// 通知状态
};

#endif