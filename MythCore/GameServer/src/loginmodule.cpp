#include "loginmodule.h"
#include "loginmessage.hxx.pb.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "gameserver.h"
#include "entity.h"
#include "objpool.h"
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
			onMessageLoginRequest(nSocketIndex, pMessage);
			break;
		}
		case ID_C2S_REQUEST_CREATE_ROLE:
		{
			onMessageCreateRoleRequest(nSocketIndex, pMessage);
			break;
		}
		case ID_C2S_REQUEST_ENTER_SCENE:
		{
			onMessageEnterSceneRequest(nSocketIndex, pMessage);
			break;
		}
		default:
			break;
	}
}

void CLoginModule::onMessageLoginRequest(uint32 nSocketIndex, Message* pMessage)
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


void CLoginModule::onIMPlayerLoginResponse(CInternalMsg* pMsg)
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
	pLoginPlayer->setRoleID(pResponse->mRoleID);
	CExchangeHead& rExchangeHead = pLoginPlayer->GetExchangeHead();
	rExchangeHead.mSocketTime = pResponse->mSocketTime;
	rExchangeHead.mSocketIndex = pResponse->mSocketIndex;

	mLoginList.Insert(pLoginPlayer->getKey(), pLoginPlayer->getObjID());


	CIMPlayerLoginResponse* pIMLoginResponse = reinterpret_cast<CIMPlayerLoginResponse*>(pMsg);
	CMessageLoginResponse tMessageLoginResponse;
	tMessageLoginResponse.set_accountid(pIMLoginResponse->mAccountID);
	tMessageLoginResponse.set_channelid(pIMLoginResponse->mChannelID);
	tMessageLoginResponse.set_worldid(pIMLoginResponse->mWorldID);
	tMessageLoginResponse.set_roleid(pIMLoginResponse->mRoleID);
	printf("CSceneJob::onTask");

	CSceneJob::Inst()->sendClientMessage(rExchangeHead, ID_S2C_RESPONSE_LOGIN, &tMessageLoginResponse);
}

void CLoginModule::onMessageCreateRoleRequest(uint32 nSocketIndex, Message* pMessage)
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
	uint64 nAccountID = pCreateRoleRequest->accountid();
	uint64 nChannelID = pCreateRoleRequest->channelid();
	uint64 nWorldID = pCreateRoleRequest->worldid();
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

	pNewRequest->mAccountID = pCreateRoleRequest->accountid();
	pNewRequest->mChannelID = pCreateRoleRequest->channelid();
	pNewRequest->mWorldID = pCreateRoleRequest->worldid();
	strncpy(pNewRequest->mRoleName, pCreateRoleRequest->rolename().c_str(), pCreateRoleRequest->rolename().size());
	CGameServer::Inst()->pushTask(emTaskType_DB, pNewRequest);

}

void CLoginModule::onIMCreateRoleResponse(CInternalMsg* pMsg)
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

	CMessageCreateRoleResponse tCreateRoleResponse;
	tCreateRoleResponse.set_result(0);
	tCreateRoleResponse.set_roleid(pCreateRoleResponse->mRoleID);
	CSceneJob::Inst()->sendClientMessage(pLoginPlayer->GetExchangeHead(), ID_S2C_RESPONSE_CREATE_ROLE, &tCreateRoleResponse);
}


void CLoginModule::onMessageEnterSceneRequest(uint32 nSocketIndex, Message* pMessage)
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


	uint64 nAccountID = pEnterSceneRequest->accountid();
	uint64 nChannelID = pEnterSceneRequest->channelid();
	uint64 nWorldID = pEnterSceneRequest->worldid();
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

	// 校验一下
	if (pLoginPlayer->getRoleID() != pEnterSceneRequest->roleid())
	{
		return;
	}

	CIMEnterSceneRequest* pNewEnterSceneRequest = reinterpret_cast<CIMEnterSceneRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_ENTER_SCENE));
	if (NULL == pNewEnterSceneRequest)
	{
		return;
	}

	CEntityPlayer* pNewPlayer = reinterpret_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Player));
	if (NULL == pNewPlayer)
	{
		return;
	}

	pNewPlayer->setRoleID(pEnterSceneRequest->roleid());

	pNewEnterSceneRequest->mRoleID = pEnterSceneRequest->roleid();
	pNewEnterSceneRequest->mAccountID = pEnterSceneRequest->accountid();
	pNewEnterSceneRequest->mChannelID = pEnterSceneRequest->channelid();
	pNewEnterSceneRequest->mWorldID = pEnterSceneRequest->worldid();
	pNewEnterSceneRequest->mPlayerEntityID = pNewPlayer->getObjID();

	CGameServer::Inst()->pushTask(emTaskType_DB, pNewEnterSceneRequest);
}

void CLoginModule::onIMEnterSceneResponse(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}

	CIMEnterSceneResponse* pEnterSceneResponse = reinterpret_cast<CIMEnterSceneResponse*>(pMsg);
	if (NULL == pEnterSceneResponse)
	{
		return;
	}

	uint64 nAccountID = pEnterSceneResponse->mAccountID;
	uint64 nChannelID = pEnterSceneResponse->mChannelID;
	uint64 nWorldID = pEnterSceneResponse->mWorldID;
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

	CMessageEnterSceneResponse tEnterSceneResponse;
	tEnterSceneResponse.set_result(0);
	CSceneJob::Inst()->sendClientMessage(pLoginPlayer->GetExchangeHead(), ID_S2C_RESPONSE_CREATE_ROLE, &tEnterSceneResponse);
}