#include "entitytimer.h"
#include "objpool.h"
#include "locallogjob.h"
CTimerList::TimeOutFunc* CTimerList::mpTimeOutFunc = NULL;
int	CTimerList::setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes)
{
	CEntityTimer* pTimer = (CEntityTimer*)CObjPool::Inst()->allocObj(emObjType_Entity_Timer);
	if (NULL == pTimer)
	{
		LOG_ERROR("allocate entity timer failure, OwerObjID: %d, Module:%d, MilliSec: %d", nOwerObjID, nModule, nMilliSec);
		return INVALID_OBJ_ID;
	}

	pTimer->mOwerObjID = nOwerObjID;
	pTimer->mElapseMilliSec = nMilliSec;
	pTimer->mInitialMilliSec = nMilliSec;
	if (NULL != pParam)
	{
		for (int i = 0; i < nParamNum && i < PARAM_NUM; ++i)
		{
			pTimer->mParam[i] = pParam[i];
		}
	}
	pTimer->mModule = nModule;
	pTimer->mSecElapse = false;
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
void CTimerList::elapseTime(unsigned int nTickOffset)
{
	std::list<CEntityTimer*>::iterator it = mTimerList.begin();
	for (; it != mTimerList.end(); )
	{
		CEntityTimer* pTimer = *it;
		// 以秒为单位流逝,直接continue
		if (pTimer->mSecElapse)
		{
			++ it;
			continue;
		}
		pTimer->mElapseMilliSec -= (int)nTickOffset;
		if (pTimer->mElapseMilliSec > 0)
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
			it = mTimerList.erase(it);
		}
		else
		{
			++ it;
		}
	}
}
