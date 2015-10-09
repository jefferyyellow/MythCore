#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__
#include "commontype.h"
#include "singleton.h"
#include "logmanager.h"
#include <stdio.h>
// some one says,the method is not precise in multi core cpu
//  http://blog.csdn.net/solstice/article/details/5196544
/// get the cpu time stamp counter
inline uint64 rdtsc()
{	
	uint64 ticks;
#ifdef MYTH_OS_WINDOWS
	ticks = uint64(__rdtsc());
#else
//	__asm__ volatile(".byte 0x0f, 0x31" : "=a" (ticks.low), "=d" (ticks.high));
#endif
	return ticks;
}

class CClockTime
{
public:
	CClockTime()
	{
		mStartTime = 0;
		mEndTime = 0;
	}
	~CClockTime(){}

public:
	inline	sint64	GetStartTime(){return mStartTime;}
	inline	void	SetStartTime(sint64 lStartTime){mStartTime = lStartTime;}

	inline	sint64	GetEndTime(){return mEndTime;}
	inline	void	SetEndTime(sint64 lEndTime){mStartTime = lEndTime;}

public:
	void	Start();
	void	End();
	sint64	GetInterval();

public:
	/// calc how long when clock the time consume
	static sint64 CalcDeviation();
	sint64	GetDeviation(){return mDeviation;}

private:
	sint64	mStartTime;
	sint64	mEndTime;

	/// Heisenberg effect,reduce deviation
	static sint64 mDeviation;
};

class CAutoClockTime : public CClockTime
{
public:
	CAutoClockTime(const char* pName)
	{
		if (NULL != pName)
		{
			strncpy(mName, pName, sizeof(mName) - 1);
			mName[sizeof(mName) - 1] = '\0';
		}
		Start();
	}
	~CAutoClockTime()
	{
		End();
		//LOG_DEBUG("default", "Name : %s, Time: %lld\n", mName, GetInterval());
		printf("Name : %s, Time: %lld\n", mName, GetInterval());
	}

private:
	char	mName[STRING_LENGTH_32];
};

class CTimeManager : public CSingleton<CTimeManager>
{
	friend class CSingleton<CTimeManager>;
private:
	CTimeManager()
	{
		Init();
	}
	~CTimeManager()
	{
		Finial();
	}

public:
	int		Init();
	void	Finial();

public:
	inline time_t	GetCurrTime(){return mCurrTime;}
#ifdef MYTH_OS_WINDOWS
	static sint64	GetQueryPerformanceFrequency(){return mQueryPerformanceFrequency;}
#endif
public:
	/// update mCurrTime to time()
	void	UpdateCurrTime();
	/// get the time in milliseconds.
	sint64	GetMSTime();
	/// get the time in microsecond
	sint64	GetUSTime();
	/** Return the offset in 10th of micro sec between the windows base time (
	 *	01-01-1601 0:0:0 UTC) and the unix base time (01-01-1970 0:0:0 UTC).
	 *	This value is used to convert windows system and file time back and
	 *	forth to unix time (aka epoch)
	 */
	static uint64 GetWindowsToUnixBaseTimeOffset();
private:
	/// cache the time from time(),no need call function time() when we want get the second from 19700101 00:00:00
	time_t	mCurrTime;
#ifdef MYTH_OS_WINDOWS
	static	sint64 mQueryPerformanceFrequency;
#endif
};


#endif
