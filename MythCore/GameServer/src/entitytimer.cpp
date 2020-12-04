#include "entitytimer.h"
#include "objpool.h"
CTimerList::TimeOutFunc* CTimerList::mpTimeOutFunc = NULL;
int	CTimerList::setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes)
{
	CEntityTimer* pTimer = (CEntityTimer*)CObjPool::Inst()->allocObj(emObjType_Entity_Timer);
	if (NULL == pTimer)
	{
		return INVALID_OBJ_ID;
	}

	pTimer->mOwerObjID = nOwerObjID;
	//pTimer->mTickCount = nMilliSec + CSceneJob::Inst()->getLastTimerTick();
	pTimer->mInitialMilliSec = nMilliSec;
	if (NULL != pParam)
	{
		for (int i = 0; i < nParamNum && i < PARAM_NUM; ++i)
		{
			pTimer->mParam[i] = pParam[i];
		}
	}
	pTimer->mModule = nModule;
	mTimerList.push_back(pTimer);
	return pTimer->getObjID();
}

void CTimerList::clearTimer(int nObjID)
{
	std::list<CEntityTimer*>::iterator it = mTimerList.begin();
	for (; it != mTimerList.end(); ++ it)
	{
		if ((*it)->getObjID() == nObjID)
		{
			CObjPool::Inst()->free(nObjID);
			mTimerList.erase(it);
			break;
		}
	}
}

void CTimerList::clearAllTimer()
{
	std::list<CEntityTimer*>::iterator it = mTimerList.begin();
	for (; it != mTimerList.end(); ++it)
	{
		CObjPool::Inst()->free((*it)->getObjID());
	}

	mTimerList.clear();
}

/// 流逝时间
void CTimerList::update(uint64 nNowTickCount)
{
	std::list<CEntityTimer*>::iterator it = mTimerList.begin();
	for (; it != mTimerList.end(); )
	{
		CEntityTimer* pTimer = *it;
		if (pTimer->mTickCount < nNowTickCount)
		{
			++ it;
			continue;
		}
		// 减少调用次数
		-- (pTimer->mCallTimes);
		if (NULL != mpTimeOutFunc)
		{
			mpTimeOutFunc(pTimer);
		}
		if (pTimer->mCallTimes <= 0)
		{
			CObjPool::Inst()->free((*it)->getObjID());
			it = mTimerList.erase(it);
		}
		else
		{
			// 重置计时器
			pTimer->mTickCount = nNowTickCount + pTimer->mInitialMilliSec;
			++ it;
		}
	}
}
