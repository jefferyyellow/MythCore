#include "loginmodule.h"
#include "loginmessage.pb.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "dbmanager.h"
CLoginModule::CLoginModule()
{

}


CLoginModule::~CLoginModule()
{

}

void CLoginModule::onClientMessage(unsigned int nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_LOGIN:
		{
			OnMessageLoginRequest(pMessage);
			break;
		}
		default:
			break;
	}
}

void CLoginModule::OnMessageLoginRequest(Message* pMessage)
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

	const char* pName = pLoginRequest->name().c_str();
	int nChannelID = pLoginRequest->channelid();
	int nWorldID = pLoginRequest->worldid();

	// 接收到客户端的消息后，转换成内部消息给DB去处理
	CIMPlayerLoginRequest* pPlayerLoginRequest = reinterpret_cast<CIMPlayerLoginRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_PLAYER_LOGIN));
	if (NULL != pPlayerLoginRequest)
	{
		pPlayerLoginRequest->mChannelID = pLoginRequest->channelid();
		pPlayerLoginRequest->mWorldID = pLoginRequest->worldid();
		strncpy(pPlayerLoginRequest->mName, pLoginRequest->name().c_str(), sizeof(pPlayerLoginRequest->mName));
		CDBManager::Inst()->pushTask(pPlayerLoginRequest);
	}
}