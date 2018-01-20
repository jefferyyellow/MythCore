#include "player.h"
#include "loginmessage.hxx.pb.h"
#include "logintester.h"
void CPlayer::onServerMessage(short nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_S2C_RESPONSE_LOGIN:
		{
			onMessageLoginResponse(pMessage);
			break;
		}
		case ID_S2C_RESPONSE_CREATE_ROLE:
		{
			onCreateRoleResponse(pMessage);
			break;
		}
		default:

			break;
	}
}

void CPlayer::loginServer()
{
	CLoginRequest tLoginRequest;
	tLoginRequest.set_name(mAccountName);
	tLoginRequest.set_channelid(mChannelID);
	tLoginRequest.set_serverid(mServerID);
	CLoginTester::Inst()->sendMessage(mTcpIndex, ID_C2S_REQUEST_LOGIN, &tLoginRequest);
}

void CPlayer::onMessageLoginResponse(Message* pMessage)
{
	char acName[256] = { 0 };
	CLoginResponse* pLoginResponse = (CLoginResponse*)pMessage;
	snprintf(acName, sizeof(acName), "hjh%d", pLoginResponse->accountid());

	int nRoleID = pLoginResponse->roleid();

	mAccountID = pLoginResponse->accountid();
	mChannelID = pLoginResponse->channelid();
	mServerID = pLoginResponse->serverid();
	mRoleID = pLoginResponse->roleid();
	if (0 == nRoleID)
	{
		// ´´½¨½Ç
		CCreateRoleRequest tCreateRoleRequest;
		tCreateRoleRequest.set_accountid(pLoginResponse->accountid());
		tCreateRoleRequest.set_channelid(pLoginResponse->channelid());
		tCreateRoleRequest.set_serverid(pLoginResponse->serverid());
		tCreateRoleRequest.set_rolename(acName);
		CLoginTester::Inst()->sendMessage(mTcpIndex, ID_C2S_REQUEST_CREATE_ROLE, &tCreateRoleRequest);
	}
	else
	{
		CEnterSceneRequest tEnterSceneRequest;
		tEnterSceneRequest.set_roleid(pLoginResponse->roleid());
		tEnterSceneRequest.set_accountid(pLoginResponse->accountid());
		tEnterSceneRequest.set_channelid(pLoginResponse->channelid());
		tEnterSceneRequest.set_serverid(pLoginResponse->serverid());
		CLoginTester::Inst()->sendMessage(mTcpIndex, ID_C2S_REQUEST_ENTER_SCENE, &tEnterSceneRequest);
	}
}

void CPlayer::onCreateRoleResponse(Message* pMessage)
{
	CCreateRoleResponse* pRoleResponse = (CCreateRoleResponse*)pMessage;

	int nResult = pRoleResponse->result();
	unsigned int nRoleID = pRoleResponse->roleid();
	mRoleID = nRoleID;

	CEnterSceneRequest tEnterSceneRequest;
	tEnterSceneRequest.set_roleid(mRoleID);
	tEnterSceneRequest.set_accountid(mAccountID);
	tEnterSceneRequest.set_channelid(mChannelID);
	tEnterSceneRequest.set_serverid(mServerID);
	CLoginTester::Inst()->sendMessage(mTcpIndex, ID_C2S_REQUEST_ENTER_SCENE, &tEnterSceneRequest);
}
