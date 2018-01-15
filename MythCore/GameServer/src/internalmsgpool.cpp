#include "internalmsgpool.h"
#include "internalmsg.h"

CInternalMsg* CInternalMsgPool::allocMsg(int nMessageID)
{
	mLock.lock();
	CInternalMsg* pMsg = NULL;
	switch (nMessageID)
	{
		case IM_REQUEST_LOCAL_LOG:
		{
			pMsg = static_cast<CInternalMsg*>(mLocalLogRequest.allocate());
			break;
		}
		default:
		{
			break;
		}
	}
	pMsg->setMsgID(nMessageID);
	mLock.unlock();
	return pMsg;
}

void CInternalMsgPool::freeMsg(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	mLock.lock();
	switch (pMsg->getMsgID())
	{
		case IM_REQUEST_LOCAL_LOG:
		{
			mLocalLogRequest.free((CIMLocalLogRequest*)pMsg);
			break;
		}
		default:
		{
			break;
		}
	}

	mLock.unlock();
}