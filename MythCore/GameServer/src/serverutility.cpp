#include "serverutility.h"

uint64 getTickCount()
{
#ifdef MYTH_OS_WINDOWS
	return GetTickCount64();
#else
	timespec tv;
	// This is not affected by system time changes.
	if (clock_gettime(CLOCK_MONOTONIC, &tv) != 0)
	{
		printf("clock_gettime return error!");
		::exit(-1);
	}
	return ((sint64)tv.tv_sec) * 1000 + (((sint64)tv.tv_nsec/*+500*/) / 1000000);
#endif
}

time_t unixTimeStamp(const char* pTime)
{
	if (NULL == pTime)
	{
		return time(NULL);
	}

	int nYear = 0;
	int nMonth = 0;
	int nDay = 0;
	int nResult = sscanf(pTime, "%d-%d-%d", &nYear, &nMonth, &nDay);
	if (nResult < 0)
	{
		return time(NULL);
	}

	int nHour = 0;
	int nMinute = 0;
	int nSecond = 0;
	const char* pDayTime = strchr(pTime, ' ');
	if (NULL != pDayTime)
	{
		++pDayTime;
		nResult = sscanf(pDayTime, "%d:%d:%d", &nHour, &nMinute, &nSecond);
		if (nResult < 0)
		{
			return time(NULL);
		}
	}


	struct tm tTm;

	tTm.tm_year = nYear - 1900;
	tTm.tm_mon = nMonth - 1;
	tTm.tm_mday = nDay;
	tTm.tm_hour = nHour;
	tTm.tm_min = nMinute;
	tTm.tm_sec = nSecond;

	return mktime(&tTm);
}

time_t timeToMorning(time_t tTime)
{
	struct tm tTmNow;
#ifdef MYTH_OS_WINDOWS
	localtime_s(&tTmNow, &tTime);
#else
	localtime_r(&tTime, &tTmNow);
#endif // MYTH_OS_WINDOWS
	int tSeconds = tTmNow.tm_hour * 60 * 60 + tTmNow.tm_min * 60 + tTmNow.tm_sec;
	return tTime - tSeconds;
}