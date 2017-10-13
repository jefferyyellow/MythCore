#include "loginmodule.h"
#include "loginmessage.hxx.pb.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "gameserver.h"
#include "entityplayer.h"
#include "objpool.h"
#include "timemanager.h"
#include "loginplayer.h"
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
	CLoginPlayer* pLoginPlayer = NULL;
	LOGIN_LIST::iterator it = mLoginList.find(rExchangeHead.mSocketIndex);
	if (it == mLoginList.end())
	{
		pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->allocObj(emObjType_LoginPlayer));
		// 分配失败
		if (NULL == pLoginPlayer)
		{
			return;
		}
		std::pair<LOGIN_LIST::iterator,bool> ret = mLoginList.insert(LOGIN_LIST::value_type(rExchangeHead.mSocketIndex, pLoginPlayer->getObjID()));
		// 插入列表失败
		if (!ret.second)
		{
			CObjPool::Inst()->free(pLoginPlayer->getObjID());
			return;
		}
		pLoginPlayer->getExchangeHead() = rExchangeHead;
		it = ret.first;
	}
	else
	{
		pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
	}
	if (NULL == pLoginPlayer)
	{
		return;
	}
	pLoginPlayer->setClientMessage(pMessage);
	pLoginPlayer->setClientMessageID(nMessageID);
}

void CLoginModule::onMessageLoginRequest(CExchangeHead& rExchangeHead, Message* pMessage)
{
	if (NULL == pMessage)
	{
		return;
	}


}


void CLoginModule::onIMPlayerLoginResponse(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}

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