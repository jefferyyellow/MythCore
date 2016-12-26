#include "internalmsgpool.h"
#include "internalmsg.h"

CInternalMsg* CInternalMsgPool::allocMsg(int nMessageID)
{
	mLock.lock();
	CInternalMsg* pMsg = NULL;
	switch (nMessageID)
	{
		case ID_REQUEST_PLAYER_LOGIN_MSG:
		{
			pMsg = (CInternalMsg*)mPlayerLoginRequest.allocate();
			break;
		}
		default:
		{
			break;
		}
	}
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
		case ID_REQUEST_PLAYER_LOGIN_MSG:
		{
			mPlayerLoginRequest.free((CPlayerLoginRequest*)pMsg);
			break;
		}
		default:
		{
			break;
		}
	}

	mLock.unlock();
}