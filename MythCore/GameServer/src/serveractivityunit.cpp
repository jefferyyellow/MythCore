#include "serveractivityunit.h"
#include "locallogjob.h"
CPhaseActivityData* CServerActivityUnit::getPhaseDataByID(int nUniqueID)
{
	for (int i = 0; i < mPhaseActivityNum; ++ i)
	{
		if (mPhaseData[i].mUniqueID == nUniqueID)
		{
			return &mPhaseData[i];
		}
	}

	return NULL;
}

void CServerActivityUnit::addPhaseData(CPhaseActivityData& rData, CPhaseActivityData& rCompleteData)
{
	if (mPhaseActivityNum >= PHASE_ACTIVITY_NUM)
	{
		LOG_ERROR("Phase activity num bigger than capacity, PhaseActivityNum: %d, Capacity: %d", mPhaseActivityNum, PHASE_ACTIVITY_NUM);
		return;
	}
	mPhaseData[mPhaseActivityNum] = rData;
	mCompleteData[mPhaseActivityNum] = rCompleteData;
	++ mPhaseActivityNum;
}
