#include "loginmodule.h"
#include "loginmessage.hxx.pb.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "gameserver.h"
#include "entityplayer.h"
#include "objpool.h"
#include "timemanager.h"
CLoginModule::CLoginModule()
:mLoginCheckTime(1000)
{

}


CLoginModule::~CLoginModule()
{

}

/// 时间函数
void CLoginModule::OnTimer(unsigned int nTickOffset)
{
	if (mLoginCheckTime.elapse(nTickOffset))
	{
		time_t tTimeNow = CTimeManager::Inst()->GetCurrTime();
		LOGIN_LIST::iterator it = mLoginList.begin();
		for (; it != mLoginList.end(); )
		{
			uint32 nObjID = it->second;
			CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
			if (NULL == pLoginPlayer)
			{
				it = mLoginList.erase(it);
				continue;
			}
			// 超时
			if (tTimeNow - pLoginPlayer->GetWaitTime() > 10)
			{
				// 断开连接
				it = mLoginList.erase(it);
				continue;
			}

			++ it;
		}
	}
}

void CLoginModule::onClientMessage(CExchangeHead& rExchangeHead, unsigned int nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_LOGIN:
		{
			onMessageLoginRequest(rExchangeHead, pMessage);
			break;
		}
		case ID_C2S_REQUEST_CREATE_ROLE:
		{
			onMessageCreateRoleRequest(rExchangeHead, pMessage);
			break;
		}
		case ID_C2S_REQUEST_ENTER_SCENE:
		{
			onMessageEnterSceneRequest(rExchangeHead, pMessage);
			break;
		}
		default:
			break;
	}
}

void CLoginModule::onMessageLoginRequest(CExchangeHead& rExchangeHead, Message* pMessage)
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
	if (NULL == pPlayerLoginRequest)
	{
		return;
	}

	// 用户名
	strncpy(pPlayerLoginRequest->mName, pLoginRequest->name().c_str(), sizeof(pPlayerLoginRequest->mName));
	// 渠道ID
	pPlayerLoginRequest->mChannelID = pLoginRequest->channelid();
	// 服务器ID
	pPlayerLoginRequest->mServerID = pLoginRequest->serverid();
	// TCP服务器消息头
	pPlayerLoginRequest->mExchangeHead = rExchangeHead;

	CGameServer::Inst()->pushTask(emTaskType_DB, pPlayerLoginRequest);
	printf("CLoginModule::OnMessageLoginRequest");
}


void CLoginModule::onIMPlayerLoginResponse(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}

	CIMPlayerLoginResponse* pResponse = reinterpret_cast<CIMPlayerLoginResponse*>(pMsg);


	uint64 nAccountID = pResponse->mAccountID;
	uint64 nChannelID = pResponse->mChannelID;
	uint64 nServerID = pResponse->mServerID;
	uint64 nKey = MAKE_LOGIN_KEY(nAccountID, nChannelID, nServerID);

	// 该玩家已经在登录列表里了
	LOGIN_LIST::iterator it = mLoginList.find(nKey);
	if (it != mLoginList.end())
	{
		return;
	}

	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->allocObj(emObjType_LoginPlayer));
	if (NULL == pLoginPlayer)
	{
		return;
	}

	pLoginPlayer->setKey(nKey);
	pLoginPlayer->setRoleID(pResponse->mRoleID);
	pLoginPlayer->GetExchangeHead() = pResponse->mExchangeHead;
	pLoginPlayer->SetWaitTime(CTimeManager::Inst()->GetCurrTime());

	mLoginList[pLoginPlayer->getKey()] = pLoginPlayer->getObjID();

	// 准备回应消息
	CIMPlayerLoginResponse* pIMLoginResponse = reinterpret_cast<CIMPlayerLoginResponse*>(pMsg);
	CMessageLoginResponse tMessageLoginResponse;
	tMessageLoginResponse.set_accountid(pIMLoginResponse->mAccountID);
	tMessageLoginResponse.set_channelid(pIMLoginResponse->mChannelID);
	tMessageLoginResponse.set_serverid(pIMLoginResponse->mServerID);
	tMessageLoginResponse.set_roleid(pIMLoginResponse->mRoleID);

	CSceneJob::Inst()->sendClientMessage(pLoginPlayer->GetExchangeHead(), ID_S2C_RESPONSE_LOGIN, &tMessageLoginResponse);
}

void CLoginModule::onMessageCreateRoleRequest(CExchangeHead& rExchangeHead, Message* pMessage)
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
	uint64 nServerID = pCreateRoleRequest->serverid();
	uint64 nKey = MAKE_LOGIN_KEY(nAccountID, nChannelID, nServerID);

	LOGIN_LIST::iterator it = mLoginList.find(nKey);
	if (it == mLoginList.end())
	{
		// 已经不存在了
		return;
	}

	uint32 nObjID = it->second;
	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
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
	pNewRequest->mServerID = pCreateRoleRequest->serverid();
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
	uint64 nServerID = pCreateRoleResponse->mServerID;
	uint64 nKey = MAKE_LOGIN_KEY(nAccountID, nChannelID, nServerID);

	LOGIN_LIST::iterator it = mLoginList.find(nKey);
	if (it == mLoginList.end())
	{
		// 已经不存在了
		return;
	}

	uint32 nObjID = it->second;
	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
	if (NULL == pLoginPlayer)
	{
		return;
	}

	CMessageCreateRoleResponse tCreateRoleResponse;
	tCreateRoleResponse.set_result(0);
	tCreateRoleResponse.set_roleid(pCreateRoleResponse->mRoleID);
	CSceneJob::Inst()->sendClientMessage(pLoginPlayer->GetExchangeHead(), ID_S2C_RESPONSE_CREATE_ROLE, &tCreateRoleResponse);
}


void CLoginModule::onMessageEnterSceneRequest(CExchangeHead& rExchangeHead, Message* pMessage)
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
	uint64 nServerID = pEnterSceneRequest->serverid();
	uint64 nKey = MAKE_LOGIN_KEY(nAccountID, nChannelID, nServerID);

	//bool bFind = mLoginList.Find(nKey, nObjID);
	LOGIN_LIST::iterator it = mLoginList.find(nKey);
	if (it == mLoginList.end())
	{
		// 已经不存在了
		return;
	}

	uint32 nObjID = it->second;
	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
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

	CEntityPlayer* pNewPlayer = reinterpret_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	if (NULL == pNewPlayer)
	{
		return;
	}

	pNewPlayer->setRoleID(pEnterSceneRequest->roleid());

	pNewEnterSceneRequest->mRoleID = pEnterSceneRequest->roleid();
	pNewEnterSceneRequest->mAccountID = pEnterSceneRequest->accountid();
	pNewEnterSceneRequest->mChannelID = pEnterSceneRequest->channelid();
	pNewEnterSceneRequest->mServerID = pEnterSceneRequest->serverid();
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
	uint64 nServerID = pEnterSceneResponse->mServerID;
	uint64 nKey = MAKE_LOGIN_KEY(nAccountID, nChannelID, nServerID);

	LOGIN_LIST::iterator it = mLoginList.find(nKey);
	if (it == mLoginList.end())
	{
		// 已经不存在了
		return;
	}

	uint32 nObjID = it->second;
	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
	if (NULL == pLoginPlayer)
	{
		return;
	}

	CMessageEnterSceneResponse tEnterSceneResponse;
	tEnterSceneResponse.set_result(0);
	CSceneJob::Inst()->sendClientMessage(pLoginPlayer->GetExchangeHead(), ID_S2C_RESPONSE_CREATE_ROLE, &tEnterSceneResponse);
}