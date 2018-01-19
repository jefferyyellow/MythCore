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