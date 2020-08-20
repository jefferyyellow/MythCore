#ifndef __DAILYACTIVITY_H__
#define __DAILYACTIVITY_H__
#define MAX_ACTIVITY_TIMES		4
#include "servercommon.h"
#include "dailyactivitytype.h"
class CDailyActTime
{
public:
	int					mID;			// 活动ID
	int					mTime;			// 时间
	int					mTimeIndex;		// 时间索引
	EmDailyActStatus	mStatus;		// 活动状态
};

class CDailyActivity
{
public:
	CDailyActivity()
	{
	}
	~CDailyActivity()
	{
	}
	
	void init();

public:
	/// 配置部分
	/// 活动类型
	short				mType;
	/// ID
	short				mID;
	/// 最小等级
	byte				mMinLevel;
	/// 最大等级
	byte				mMaxLevel;
	/// 开始时间
	int					mStartTime[MAX_ACTIVITY_TIMES];
	/// 结束时间 
	int					mEndTime[MAX_ACTIVITY_TIMES];
	/// 通知时间
	int					mNoticeTime[MAX_ACTIVITY_TIMES];

public:
	EmDailyActStatus GetStatus() const { return mStatus; }
	void SetStatus(EmDailyActStatus nValue) { mStatus = nValue; }

public:
	/// 活动开启
	void start();
	/// 活动结束的清理
	void end();
	/// 通知
	void notice();

public:
	/// autocode, don't edit!!!
    EmDailyActStatus getStatus(){ return mStatus;}
    void setStatus(EmDailyActStatus value){ mStatus = value;}
	/// end autocode
private:
	/// 数据部分 default:emDailyActStatus_None
	EmDailyActStatus	mStatus;
};


#endif