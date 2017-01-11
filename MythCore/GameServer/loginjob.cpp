#include "loginjob.h"
#include "internalmsgpool.h"
#include "objpool.h"
#include "gameserver.h"
void CLoginJob::doing(uint32 uParam)
{
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}
		onTask(pIMMsg);

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
	}
}

void CLoginJob::onTask(CInternalMsg* pMsg)
{
	switch (pMsg->getMsgID())
	{
		case IM_RESPONSE_PLAYER_LOGIN:
		{
			onIMPlayerLoginResponse(pMsg);
			break;
		}
		case IM_REQUEST_CREATE_ROLE:
		{
			onIMCreateRoleRequest(pMsg);
			break;
		}
		case IM_RESPONSE_CREATE_ROLE:
		{
			OnIMCreateRoleResponse(pMsg);
			break;
		}
		default:
			break;
	}
}

void CLoginJob::onIMPlayerLoginResponse(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}

	CIMPlayerLoginResponse* pResponse = reinterpret_cast<CIMPlayerLoginResponse*>(pMsg);

	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::CreateInst()->allocObj(emObjType_LoginPlayer));
	if (NULL == pLoginPlayer)
	{
		return;
	}
	uint64 nAccountID = pResponse->mAccountID;
	uint64 nChannelID = pResponse->mChannelID;
	uint64 nWorldID = pResponse->mWorldID;
	pLoginPlayer->setKey(MAKE_LOGIN_KEY(nAccountID, nChannelID, nWorldID));
	pLoginPlayer->setSocketIndex(pResponse->mSocketIndex);
	pLoginPlayer->setSocketTime(pResponse->mSocketTime);

	mLoginList.Insert(pLoginPlayer->getKey(), pLoginPlayer->getObjID());

	CIMPlayerLoginResponse* pNewResponse = reinterpret_cast<CIMPlayerLoginResponse*>(CInternalMsgPool::CreateInst()->allocMsg(IM_RESPONSE_PLAYER_LOGIN));
	if (NULL == pNewResponse)
	{
		return;
	}
	pNewResponse->mAccountID = pResponse->mAccountID;
	pNewResponse->mChannelID = pResponse->mChannelID;
	pNewResponse->mWorldID = pResponse->mWorldID;
	pNewResponse->mRoleID = pResponse->mRoleID;
	pNewResponse->mSocketIndex = pResponse->mSocketIndex;
	pNewResponse->mSocketTime = pResponse->mSocketTime;

	CGameServer::Inst()->pushTask(emTaskType_Scene, pNewResponse);
}

void CLoginJob::onIMCreateRoleRequest(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	CIMCreateRoleRequest* pIMCreateRoleRequest = reinterpret_cast<CIMCreateRoleRequest*>(pMsg);
	if (NULL == pIMCreateRoleRequest)
	{
		return;
	}

	uint64 nAccountID = pIMCreateRoleRequest->mAccountID;
	uint64 nChannelID = pIMCreateRoleRequest->mChannelID;
	uint64 nWorldID = pIMCreateRoleRequest->mWorldID;
	uint64 nKey = MAKE_LOGIN_KEY(nAccountID, nChannelID, nWorldID);
	
	uint32 nObjID = 0;
	bool bFind = mLoginList.Find(nKey, nObjID);
	if (!bFind || nObjID == 0)
	{
		// 已经不存在了
		return;
	}

	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::CreateInst()->getObj(nObjID));
	if (NULL == pLoginPlayer)
	{
		return;
	}

	CIMCreateRoleRequest* pNewRequest = reinterpret_cast<CIMCreateRoleRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_CREATE_ROLE));
	if (NULL == pNewRequest)
	{
		return;
	}

	pNewRequest->mAccountID = pIMCreateRoleRequest->mAccountID;
	pNewRequest->mChannelID = pIMCreateRoleRequest->mChannelID;
	pNewRequest->mWorldID = pIMCreateRoleRequest->mWorldID;
	strncpy(pNewRequest->mRoleName, pIMCreateRoleRequest->mRoleName, sizeof(pNewRequest->mRoleName));
	CGameServer::Inst()->pushTask(emTaskType_DB, pNewRequest);

}

void CLoginJob::OnIMCreateRoleResponse(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	CIMCreateRoleResponse* pCreateRoleResponse = reinterpret_cast<CIMCreateRoleResponse*>(pMsg);


	uint64 nAccountID = pCreateRoleResponse->mAccountID;
	uint64 nChannelID = pCreateRoleResponse->mChannelID;
	uint64 nWorldID = pCreateRoleResponse->mWorldID;
	uint64 nKey = MAKE_LOGIN_KEY(nAccountID, nChannelID, nWorldID);

	uint32 nObjID = 0;
	bool bFind = mLoginList.Find(nKey, nObjID);
	if (!bFind || nObjID == 0)
	{
		// 已经不存在了
		return;
	}

	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::CreateInst()->getObj(nObjID));
	if (NULL == pLoginPlayer)
	{
		return;
	}

	CIMCreateRoleResponse* pNewRoleResponse = reinterpret_cast<CIMCreateRoleResponse*>(CInternalMsgPool::Inst()->allocMsg(IM_RESPONSE_CREATE_ROLE));
	if (NULL == pNewRoleResponse)
	{
		return;
	}
	pNewRoleResponse->mRoleID = pCreateRoleResponse->mRoleID;
	pNewRoleResponse->mSocketIndex = pLoginPlayer->getSocketIndex();
	CGameServer::Inst()->pushTask(emTaskType_Scene, pNewRoleResponse);
}