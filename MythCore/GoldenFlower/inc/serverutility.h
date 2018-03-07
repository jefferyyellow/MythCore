#ifndef __SERVERUTILITY_H__
#define __SERVERUTILITY_H__
#include "servercommon.h"
uint64		getTickCount();
time_t		unixTimeStamp(const char* pTime);
time_t		timeToMorning(time_t tTime);
#endif