#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__
#include "commontype.h"
#include "singleton.h"
#include "logmanager.h"
#include <stdio.h>

namespace Myth
{
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
		inline	sint64	getStartTime(){return mStartTime;}
		inline	void	setStartTime(sint64 lStartTime){mStartTime = lStartTime;}

		inline	sint64	getEndTime(){return mEndTime;}
		inline	void	setEndTime(sint64 lEndTime){mStartTime = lEndTime;}

	public:
		void	start();
		void	end();
		sint64	getInterval();

	public:
		/// calc how long when clock the time consume
		static sint64 calcDeviation();
		sint64	getDeviation(){return mDeviation;}

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
			start();
		}
		~CAutoClockTime()
		{
			end();
			//LOG_DEBUG("default", "Name : %s, Time: %lld\n", mName, GetInterval());
			printf("Name : %s, Time: %lld\n", mName, getInterval());
		}

	private:
		char	mName[STRING_LENGTH_32];
	};

	/// 自动重置计算器
	class CAutoResetTimer
	{
	public:
		CAutoResetTimer(int nMaxTime)
			:mMaxTime(nMaxTime), mLeftTime(nMaxTime)
		{
		}

	public:
		time_t getLeftTime() const { return mLeftTime; }
		void setLeftTime(time_t nValue) { mLeftTime = nValue; }

		time_t getMaxTime() const { return mMaxTime; }
		void setMaxTime(time_t nValue) { mMaxTime = nValue; }

		inline bool elapse(int vInterval)
		{
			mLeftTime -= vInterval; 
			// 如果剩余的时间小于0，自动重置到最大值，并返回true
			return mLeftTime <= 0 ? mLeftTime = mMaxTime, true : false;
		}

	private:
		time_t mLeftTime;			// 剩余时间(毫秒)
		time_t mMaxTime;			// 最大的时间(毫秒)
	};

	class CTimeManager : public CSingleton<CTimeManager>
	{
		friend class CSingleton < CTimeManager > ;
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
	#ifdef MYTH_OS_WINDOWS
		sint64	GetQueryPerformanceFrequency(){return mQueryPerformanceFrequency;}
	#endif
	public:
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

		inline time_t	getCurrTime(){ return mCurrTime; }
		void			setCurrTime(time_t tTime){mCurrTime = tTime;}

		uint64			getTickCount()const{ return mTickCount; }
		void			setTickCount(uint64 tTickCount){mTickCount = tTickCount;}

		tm&				getTmNow(){return mTmNow;}
		void			setTmNow(time_t tTimeNow)
		{
#ifdef MYTH_OS_WINDOWS
			localtime_s(&mTmNow, &tTimeNow);
#else
			localtime_r(&tTimeNow, &mTmNow);
#endif // MYTH_OS_WINDOWS
		}
	private:
		/// cache the time from time(),no need call function time() when we want get the second from 19700101 00:00:00
	#ifdef MYTH_OS_WINDOWS
		sint64 mQueryPerformanceFrequency;
	#endif
		time_t					mCurrTime;
		uint64					mTickCount;
		struct tm				mTmNow;
	};

}



#endif
