#include "perfmanager.h"
void CPerfData::addCall(int nElapseTime)
{
	++ mCallTimes;
	mCostTime += nElapseTime;
	if (mMaxTime < nElapseTime)
	{
		mMaxTime = nElapseTime;
	}
	if (nElapseTime < mMinTime)
	{
		mMinTime = nElapseTime;
	}
}

void CPerfData::reset()
{
	mCallTimes = 0;
	mCostTime = 0;
	mMaxTime =  0;
	mMinTime = PERF_MIN_TIME;
}