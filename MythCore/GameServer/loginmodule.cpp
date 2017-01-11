#include "loginmodule.h"
#include "loginmessage.hxx.pb.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "gameserver.h"
CLoginModule::CLoginModule()
{

}


CLoginModule::~CLoginModule()
{

}

void CLoginModule::onClientMessage(uint32 nSocketIndex, unsigned int nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_LOGIN:
		{
			OnMessageLoginRequest(nSocketIndex, pMessage);
			break;
		}
		case ID_C2S_REQUEST_CREATE_ROLE:
		{
			OnMessageCreateRoleRequest(nSocketIndex, pMessage);
			break;
		}
		case ID_C2S_REQUEST_ENTER_SCENE:
		{
			OnMessageEnterSceneRequest(nSocketIndex, pMessage);
			break;
		}
		default:
			break;
	}
}

void CLoginModule::OnMessageLoginRequest(uint32 nSocketIndex, Message* pMessage)
{
	if (NULL == pMessage)
	{
		return;
	}

	CMessageLoginRequest* pLoginRequest = reinterpret_cast<CMessageLoginRequest*>(pMessage);
	if (NULL == pLoginRequest)
	{
		return;
	}

	// 接收到客户端的消息后，转换成内部消息给DB去处理
	CIMPlayerLoginRequest* pPlayerLoginRequest = reinterpret_cast<CIMPlayerLoginRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_PLAYER_LOGIN));
	if (NULL != pPlayerLoginRequest)
	{
		pPlayerLoginRequest->mChannelID = pLoginRequest->channelid();
		pPlayerLoginRequest->mWorldID = pLoginRequest->worldid();
		pPlayerLoginRequest->mSocketIndex = nSocketIndex;
		strncpy(pPlayerLoginRequest->mName, pLoginRequest->name().c_str(), sizeof(pPlayerLoginRequest->mName));
		CGameServer::Inst()->pushTask(emTaskType_DB, pPlayerLoginRequest);
		printf("CLoginModule::OnMessageLoginRequest");
	}
}

void CLoginModule::OnMessageCreateRoleRequest(uint32 nSocketIndex, Message* pMessage)
{
	if (NULL == pMessage)
	{
		return;
	}

	CMessageCreateRoleRequest* pCreateRoleRequest = reinterpret_cast<CMessageCreateRoleRequest*>(pMessage);
	if (NULL == pCreateRoleRequest)
	{
		return;
	}
	CIMCreateRoleRequest* pIMCreateRoleRequest = (CIMCreateRoleRequest*)CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_CREATE_ROLE);
	if (NULL == pCreateRoleRequest)
	{
		return;
	}

	pIMCreateRoleRequest->mAccountID = pCreateRoleRequest->accountid();
	pIMCreateRoleRequest->mChannelID = pCreateRoleRequest->channelid();
	pIMCreateRoleRequest->mWorldID = pCreateRoleRequest->worldid();
	strncpy(pIMCreateRoleRequest->mRoleName, pCreateRoleRequest->rolename().c_str(), 
		sizeof(pIMCreateRoleRequest->mRoleName) - 1);

	CGameServer::Inst()->pushTask(emTaskType_Login, pIMCreateRoleRequest);
}

void CLoginModule::OnMessageEnterSceneRequest(uint32 nSocketIndex, Message* pMessage)
{
	if (NULL == pMessage)
	{
		return;
	}

	CMessageEnterSceneRequest* pEnterSceneRequest = reinterpret_cast<CMessageEnterSceneRequest*>(pMessage);
	if (NULL == pEnterSceneRequest)
	{
		return;
	}
	uint32 nRoleID = pEnterSceneRequest->roleid();

}