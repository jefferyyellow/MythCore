#ifndef __DAILYACTIVITY_H__
#define __DAILYACTIVITY_H__
#define MAX_ACTIVITY_TIMES		4
#include "servercommon.h"
enum EmDailyActID
{
	emDailyActID_None			= 0,
	emDailyActID_XXX			= 1,
	emDailyActIDMax
};

class CDailyActivity
{
public:
	/// ID
	short				mID;
	/// ��С�ȼ�
	byte				mMinLevel;
	/// ���ȼ�
	byte				mMaxLevel;
	/// ��ʼʱ��
	time_t				mStartTime[MAX_ACTIVITY_TIMES];
	/// ����ʱ�� 
	time_t				mEndTime[MAX_ACTIVITY_TIMES];
};

#endif