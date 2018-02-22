#include "serveractivityunit.h"
#include "locallogjob.h"
#include "serveractmodule.h"
#include "serveractivity.h"
CServerActData* CServerActivityUnit::getServerActData(int nUniqueID)
{
	for (int i = 0; i < mActivityNum; ++i)
	{
		if (mActivityData[i].mUniqueID == nUniqueID)
		{
			return &mActivityData[i];
		}
	}

	return NULL;
}

void CServerActivityUnit::addServerActData(CServerActData& rData)
{
	if (mActivityNum >= PHASE_ACTIVITY_NUM)
	{
		LOG_ERROR("Phase activity num bigger than capacity, PhaseActivityNum: %d, Capacity: %d", mActivityNum, PHASE_ACTIVITY_NUM);
		return;
	}
	mActivityData[mActivityNum] = rData;
	++mActivityNum;
}


int CServerActivityUnit::removeServerActData(int nPos)
{
	if (nPos >= mActivityNum || nPos < 0)
	{
		return mActivityNum;
	}

	if (mActivityNum <= 0)
	{
		return mActivityNum;
	}

	mActivityData[nPos] = mActivityData[mActivityNum - 1];
	--mActivityNum;
	return nPos;
}

void CServerActivityUnit::checkAllServerAct()
{
	for (int i = 0; i < mActivityNum; )
	{
		CServerActivity* pActivity = CServerActModule::Inst()->getServerActivity(mActivityData[i].getUniqueID());
		if (NULL == pActivity || pActivity->getStartTime() != mActivityData[i].getStartTime())
		{
			i = removeServerActData(i);
		}
		else
		{
			++i;
		}
	}
}