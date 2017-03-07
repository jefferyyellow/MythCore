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
			pMsg = reinterpret_cast<CInternalMsg*>(mLocalLogRequest.allocate());
			break;
		}
		case IM_REQUEST_PLAYER_LOGIN:
		{
			pMsg = reinterpret_cast<CInternalMsg*>(mPlayerLoginRequest.allocate());
			break;
		}
		case IM_RESPONSE_PLAYER_LOGIN:
		{
			pMsg = reinterpret_cast<CInternalMsg*>(mPlayerLoginResponse.allocate());
			break;
		}
		case IM_REQUEST_CREATE_ROLE:
		{
			pMsg = reinterpret_cast<CInternalMsg*>(mCreateRoleRequest.allocate());
			break;
		}
		case IM_RESPONSE_CREATE_ROLE:
		{
			pMsg = reinterpret_cast<CInternalMsg*>(mCreateRoleResponse.allocate());
			break;
		}
		case IM_REQUEST_ENTER_SCENE:
		{
			pMsg = reinterpret_cast<CInternalMsg*>(mEnterSceneRequest.allocate());
			break;
		}
		case IM_RESPONSE_ENTER_SCENE:
		{
			pMsg = reinterpret_cast<CInternalMsg*>(mEnterSceneResponse.allocate());
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
		case IM_REQUEST_PLAYER_LOGIN:
		{
			mPlayerLoginRequest.free((CIMPlayerLoginRequest*)pMsg);
			break;
		}
		case IM_RESPONSE_PLAYER_LOGIN:
		{
			mPlayerLoginResponse.free((CIMPlayerLoginResponse*)pMsg);
			break;
		}
		case IM_REQUEST_CREATE_ROLE:
		{
			mCreateRoleRequest.free((CIMCreateRoleRequest*)pMsg);
			break;
		}
		case IM_RESPONSE_CREATE_ROLE:
		{
			mCreateRoleResponse.free((CIMCreateRoleResponse*)pMsg);
			break;
		}
		case IM_REQUEST_ENTER_SCENE:
		{
			mEnterSceneRequest.free((CIMEnterSceneRequest*)pMsg);
			break;
		}
		case IM_RESPONSE_ENTER_SCENE:
		{
			mEnterSceneResponse.free((CIMEnterSceneResponse*)pMsg);
			break;
		}
		default:
		{
			break;
		}
	}

	mLock.unlock();
}