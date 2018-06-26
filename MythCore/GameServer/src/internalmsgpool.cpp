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
			pMsg = static_cast<CInternalMsg*>(mLocalLogRequestPool.allocate());
			break;
		}
		case IM_REQUEST_PLAT_LOG:
		{
			pMsg = static_cast<CInternalMsg*>(mPlatLogRequestPool.allocate());
			break;
		}
		case IM_REQUEST_PLAT_WEB:
		{
			pMsg = static_cast<CInternalMsg*>(mPlatWebRequestPool.allocate());
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
			mLocalLogRequestPool.free((CIMLocalLogRequest*)pMsg);
			break;
		}
		case IM_REQUEST_PLAT_LOG:
		{
			mPlatLogRequestPool.free((CIMPlatLogRequest*)pMsg);
			break;
		}
		case IM_REQUEST_PLAT_WEB:
		{
			mPlatWebRequestPool.free((CIMPlatWebRequest*)pMsg);
			break;
		}
		default:
		{
			break;
		}
	}

	mLock.unlock();
}