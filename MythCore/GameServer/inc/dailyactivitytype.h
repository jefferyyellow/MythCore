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
	emDailyActStatus_None = 0,	// ��״̬
	emDailyActStatus_Start = 1,	// ��ʼ״̬
	emDailyActStatus_End = 2,	// ����״̬
	emDailyActStatus_Notice = 3,	// ֪ͨ״̬
};

#endif