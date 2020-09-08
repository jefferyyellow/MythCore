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
		case IM_RESPONSE_PLAT_WEB:
		{
			pMsg = static_cast<CInternalMsg*>(mPlatWebResponsePool.allocate());
			break;
		}
		case IM_REQUEST_UPDATE_RANK:
		{
			pMsg = static_cast<CInternalMsg*>(mUpdateRankRequestPool.allocate());
			break;
		}
		case IM_RESPONSE_UPDATE_RANK:
		{
			pMsg = static_cast<CInternalMsg*>(mUpdateRankResponsePool.allocate());
			break;
		}
		case IM_REQUEST_GET_RANK_INFO:
		{
			pMsg = static_cast<CInternalMsg*>(mGetRankInfoRequestPool.allocate());
			break;
		}
		case IM_RESPONSE_GET_RANK_INFO:
		{
			pMsg = static_cast<CInternalMsg*>(mGetRankInfoResponsePool.allocate());
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
		case IM_RESPONSE_PLAT_WEB:
		{
			mPlatWebResponsePool.free((CIMPlatWebResponse*)pMsg);
			break;
		}
		case IM_REQUEST_UPDATE_RANK:
		{
			mUpdateRankRequestPool.free((CIMUpdateRankRequest*)pMsg);
			break;
		}
		case IM_RESPONSE_UPDATE_RANK:
		{
			mUpdateRankResponsePool.free((CIMUpdateRankResponse*)pMsg);
			break;
		}
		case IM_REQUEST_GET_RANK_INFO:
		{
			mGetRankInfoRequestPool.free((CIMGetRankInfoRequest*)pMsg);
			break;
		}
		case IM_RESPONSE_GET_RANK_INFO:
		{
			mGetRankInfoResponsePool.free((CIMGetRankInfoResponse*)pMsg);
			break;
		}
		default:
		{
			break;
		}
	}

	mLock.unlock();
}