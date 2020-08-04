#include "timeutility.h"
#include <time.h>

namespace Myth
{
	sint64 CClockTime::mDeviation = 0;
	void CClockTime::start()
	{
	#ifdef MYTH_OS_WINDOWS
		LARGE_INTEGER tCounter;
		QueryPerformanceCounter(&tCounter);
		mStartTime = tCounter.QuadPart;
	#else
		timespec tTv;
		// clock_gettime need linux kernel version 2.6 or above
		clock_gettime(CLOCK_MONOTONIC, &tTv);
		mStartTime = (sint64)tTv.tv_sec * (sint64)1000000 + ((sint64)tTv.tv_nsec / (sint64)1000);
	#endif
	}

	void CClockTime::end()
	{
	#ifdef MYTH_OS_WINDOWS
		LARGE_INTEGER tCounter;
		QueryPerformanceCounter(&tCounter);
		mEndTime = tCounter.QuadPart;
	#else
		timespec tTv;
		// clock_gettime need linux kernel version 2.6 or above
		clock_gettime(CLOCK_MONOTONIC, &tTv);
		mEndTime = (sint64)tTv.tv_sec * (sint64)1000000 + ((sint64)tTv.tv_nsec / (sint64)1000);
	#endif
	}

	sint64 CClockTime::getInterval()
	{
	#ifdef MYTH_OS_WINDOWS
		return (mEndTime - mStartTime) * 1000000 / CTimeUtility::GetQueryPerformanceFrequency();
	#else
		return (mEndTime - mStartTime)	;
	#endif
	}

	sint64 CClockTime::calcDeviation()
	{
		CClockTime tObserver;
		CClockTime tMeasure;
		tObserver.start();
		for (int i = 0; i < 10000; ++ i)
		{
			tMeasure.start();
			tMeasure.end();
		}
		tObserver.end();
		// attation >> 1
		mDeviation = (tObserver.getInterval() >> 1) / 10000;
		return mDeviation;
	}

#ifdef MYTH_OS_WINDOWS
	sint64 CTimeUtility::mQueryPerformanceFrequency = 0;
#endif
	int	CTimeUtility::init()
	{
	#ifdef MYTH_OS_WINDOWS
		LARGE_INTEGER lFrequency;
		QueryPerformanceFrequency(&lFrequency);
		mQueryPerformanceFrequency = lFrequency.QuadPart;
		if (0 == mQueryPerformanceFrequency)
		{
			mQueryPerformanceFrequency = 1;
		}
	#endif
		return 0;
	}

	void CTimeUtility::finial()
	{

	}

	/// get the time in milliseconds.
	sint64 CTimeUtility::GetMSTime()
	{
	#ifdef MYTH_OS_WINDOWS
		LARGE_INTEGER tCounter;
		QueryPerformanceCounter(&tCounter);
		tCounter.QuadPart *= (LONGLONG)1000L;
		tCounter.QuadPart /= mQueryPerformanceFrequency;
		return tCounter.QuadPart;
	#else
		timespec tTv;
		// clock_gettime need linux kernel version 2.6 or above
		if (clock_gettime(CLOCK_MONOTONIC, &tTv) != 0)
		{
			return 0;
		}
		return (sint64)tTv.tv_sec * (sint64)1000 + ((sint64)tTv.tv_nsec / (sint64)1000000);
	#endif
	}

	/// get the time in microsecond
	sint64 CTimeUtility::GetUSTime()
	{
	#ifdef MYTH_OS_WINDOWS
		LARGE_INTEGER tCounter;
		QueryPerformanceCounter(&tCounter);
		tCounter.QuadPart *= (LONGLONG)1000000L;
		tCounter.QuadPart /= mQueryPerformanceFrequency;
		return tCounter.QuadPart;
	#else
		timespec tTv;
		// clock_gettime need linux kernel version 2.6 or above
		if (clock_gettime(CLOCK_MONOTONIC, &tTv) != 0)
		{
			return 0;
		}
		return (sint64)tTv.tv_sec * (sint64)1000000 + ((sint64)tTv.tv_nsec / (sint64)1000);
	#endif
	}

	/** Return the offset in 10th of micro sec between the windows base time (
		*	01-01-1601 0:0:0 UTC) and the unix base time (01-01-1970 0:0:0 UTC).
		*	This value is used to convert windows system and file time back and
		*	forth to unix time (aka epoch)
		*	the seconds different from (01-01-1970 0:0:0 UTC) - (01-01-1601 0:0:0 UTC)
		*/
	uint64 CTimeUtility::GetWindowsToUnixBaseTimeOffset()
	{
	#ifdef MYTH_OS_WINDOWS
		// compute the offset to convert windows base time into unix time (aka epoch)
		// build a WIN32 system time for jan 1, 1970
		SYSTEMTIME baseTime;
		baseTime.wYear = 1970;
		baseTime.wMonth = 1;
		baseTime.wDayOfWeek = 0;
		baseTime.wDay = 1;
		baseTime.wHour = 0;
		baseTime.wMinute = 0;
		baseTime.wSecond = 0;
		baseTime.wMilliseconds = 0;

		FILETIME baseFileTime = {0,0};
		// convert it into a FILETIME value
		SystemTimeToFileTime(&baseTime, &baseFileTime);
		return baseFileTime.dwLowDateTime | (uint64(baseFileTime.dwHighDateTime)<<32);
	#else
		return 0;
	#endif
	}

}
