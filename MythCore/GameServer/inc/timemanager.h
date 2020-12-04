#ifndef __SVRTIMEMANAGER_H__
#define __SVRTIMEMANAGER_H__
#include "commontype.h"
#include "singleton.h"
using namespace Myth;
class CTimeManager : public CSingleton<CTimeManager>
{
	friend class CSingleton<CTimeManager>;
private:
	CTimeManager()
	{
		mCurrTime = 0;
	}

	~CTimeManager()
	{

	}

public:
	///// 将时间转化为日期为当天的时间戳,1530表示服务器时间当天的15：30
	//time_t			time2TimeStamp(int nTime);
	/// 将日期转化为时间戳,20200804转化成2020年8月4日凌晨0点0分0秒的时间戳
	time_t			date2TimeStamp(int nTime);
	/// 是否是同一天
	bool			checkSameDay(time_t nOldTime, time_t nNewTime);

public:
	inline time_t	getCurrTime(){ return mCurrTime; }
	void			setCurrTime(time_t tTime){ mCurrTime = tTime; }

private:
	/// 当前的时间(游戏时间)
	time_t			mCurrTime;
};
#endif