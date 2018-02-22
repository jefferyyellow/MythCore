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
	/// 最小等级
	byte				mMinLevel;
	/// 最大等级
	byte				mMaxLevel;
	/// 开始时间
	time_t				mStartTime[MAX_ACTIVITY_TIMES];
	/// 结束时间 
	time_t				mEndTime[MAX_ACTIVITY_TIMES];
};

#endif