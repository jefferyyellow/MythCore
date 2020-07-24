#ifndef __TIMEUNIT_H__
#define __TIMEUNIT_H__
#include "playersubunit.h"
#include "servercommon.h"
#include "entitytimer.h"
#include "messagefactory.h"

class CTimeUnit : public CPlayerSubUnit
{
public:
	CTimeUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		init();
	}
	~CTimeUnit(){}

	void init()
	{
		mLastOffTime = 0;
		mOnTime = 0;
		mLastSaveTime = 0;
		mNewDayTime = 0;
	}

public:
	/// 处理心跳
	void	onHeartBeatRequest(Message* pMessage);

public:
	/// autocode don't edit!!!
	time_t getLastOffTime(){ return mLastOffTime; }
	void setLastOffTime(time_t value){ mLastOffTime = value; }

	time_t getOnTime(){ return mOnTime; }
	void setOnTime(time_t value){ mOnTime = value; }

	time_t getLastSaveTime(){ return mLastSaveTime; }
	void setLastSaveTime(time_t value){ mLastSaveTime = value; }

	CTimerList& getTimerList(){ return mTimerList; }

	time_t getNewDayTime(){ return mNewDayTime; }
	void setNewDayTime(time_t tNewDayTime){ mNewDayTime = tNewDayTime; }
	/// end autocode
private:
	/// 上次下线时间
	time_t			mLastOffTime;
	/// 这次上线时间
	time_t			mOnTime;
	/// 上次存盘的时间
	time_t			mLastSaveTime;
	///	计时器列表
	CTimerList		mTimerList;
	/// 新的一天计算时间
	time_t			mNewDayTime;
	///  心跳时间
	time_t			mHeartBeatTime;
	///  心跳周期时间
	time_t			mHeartBeatCheckTime;
	///	 心跳过速次数
	time_t			mHeartBeatErrCount;
};
#endif