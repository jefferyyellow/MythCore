#include "loginplayer.h"
#include "loginmessage.hxx.pb.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "gameserver.h"
#include "objpool.h"
#include "entityplayer.h"

void CLoginPlayer::init()
{
	mStateMachine.init(this, emLoginState_None);
	mStateMachine.addState(emLoginState_None,			10, &CLoginPlayer::processStateNone);
	mStateMachine.addState(emLoginState_AccountVerify,	10, &CLoginPlayer::processAccountVerify);
	mStateMachine.addState(emLoginState_WaitCreateRole, 10, &CLoginPlayer::processWaitCreateRole);
	mStateMachine.addState(emLoginState_CreateRoleing,	10, &CLoginPlayer::processCreateRoleing);
	mStateMachine.addState(emLoginState_WaitEnterGame,	10, &CLoginPlayer::processWaitEnterGame);
	mStateMachine.addState(emLoginState_Playing,		10, &CLoginPlayer::processWaitPlaying);
}

void CLoginPlayer::checkState()
{
	mStateMachine.checkState();
}

int CLoginPlayer::processStateNone()
{
	if (NULL == mClientMessage || ID_C2S_REQUEST_LOGIN != mClientMessageID)
	{
		return -1;
	}

	CMessageLoginRequest* pLoginRequest = reinterpret_cast<CMessageLoginRequest*>(mClientMessage);
	if (NULL == pLoginRequest)
	{
		return -1;
	}

	// 接收到客户端的消息后，转换成内部消息给DB去处理
	CIMPlayerLoginRequest* pPlayerLoginRequest = reinterpret_cast<CIMPlayerLoginRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_PLAYER_LOGIN));
	if (NULL == pPlayerLoginRequest)
	{
		return -1;
	}

	setAccountName(pPlayerLoginRequest->mName);
	setChannelID(pPlayerLoginRequest->mChannelID);
	setServerID(pPlayerLoginRequest->mServerID);

	// 用户名
	strncpy(pPlayerLoginRequest->mName, pLoginRequest->name().c_str(), sizeof(pPlayerLoginRequest->mName));
	setPlayerLoginMsg((CIMPlayerLoginMsg*)pLoginRequest);

	CGameServer::Inst()->pushTask(emTaskType_DB, pPlayerLoginRequest);
	printf("CLoginModule::OnMessageLoginRequest");

	return emLoginState_AccountVerify;
}

int CLoginPlayer::processAccountVerify()
{
	if (NULL == mDBMessage || IM_RESPONSE_PLAYER_LOGIN != mClientMessageID)
	{
		return -1;
	}

	// 准备回应消息
	CIMPlayerLoginResponse* pIMLoginResponse = reinterpret_cast<CIMPlayerLoginResponse*>(mDBMessage);
	if (mServerID != pIMLoginResponse->mServerID 
		|| mChannelID != pIMLoginResponse->mChannelID
		|| 0 != strncmp(pIMLoginResponse->mName, mAccountName, sizeof(mAccountName)- 1))
	{
		return -1;
	}

	CMessageLoginResponse tMessageLoginResponse;
	tMessageLoginResponse.set_accountid(pIMLoginResponse->mAccountID);
	tMessageLoginResponse.set_channelid(pIMLoginResponse->mChannelID);
	tMessageLoginResponse.set_serverid(pIMLoginResponse->mServerID);
	tMessageLoginResponse.set_roleid(pIMLoginResponse->mRoleID);

	CSceneJob::Inst()->sendClientMessage(mExchangeHead, ID_S2C_RESPONSE_LOGIN, &tMessageLoginResponse);
	if (pIMLoginResponse->mRoleID == 0)
	{
		return emLoginState_WaitCreateRole;
	}
	else
	{
		return emLoginState_WaitEnterGame;
	}
}

int CLoginPlayer::processWaitCreateRole()
{
	if (NULL == mClientMessage || ID_C2S_REQUEST_CREATE_ROLE != mClientMessageID)
	{
		return -1;
	}
	CMessageCreateRoleRequest* pCreateRoleRequest = reinterpret_cast<CMessageCreateRoleRequest*>(mClientMessage);
	if (NULL == pCreateRoleRequest)
	{
		return -1;
	}

	if (mServerID != pCreateRoleRequest->serverid()
		|| mChannelID != pCreateRoleRequest->channelid()
		|| mAccountID != pCreateRoleRequest->accountid())
	{
		return -1;
	}

	CIMCreateRoleRequest* pNewRequest = reinterpret_cast<CIMCreateRoleRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_CREATE_ROLE));
	if (NULL == pNewRequest)
	{
		return -1;
	}

	setPlayerLoginMsg(pNewRequest);
	strncpy(pNewRequest->mRoleName, pCreateRoleRequest->rolename().c_str(), pCreateRoleRequest->rolename().size());
	CGameServer::Inst()->pushTask(emTaskType_DB, pNewRequest);

	return emLoginState_CreateRoleing;
}

int CLoginPlayer::processCreateRoleing()
{
	if (NULL == mDBMessage || IM_RESPONSE_CREATE_ROLE != mDBMessageID)
	{
		return -1;
	}
	CIMCreateRoleResponse* pCreateRoleResponse = reinterpret_cast<CIMCreateRoleResponse*>(mDBMessage);
	if (mServerID != pCreateRoleResponse->mServerID
		|| mChannelID != pCreateRoleResponse->mChannelID
		|| mAccountID != pCreateRoleResponse->mAccountID)
	{
		return -1;
	}


	CMessageCreateRoleResponse tCreateRoleResponse;
	tCreateRoleResponse.set_result(0);
	tCreateRoleResponse.set_roleid(pCreateRoleResponse->mRoleID);
	CSceneJob::Inst()->sendClientMessage(mExchangeHead, ID_S2C_RESPONSE_CREATE_ROLE, &tCreateRoleResponse);

	return emLoginState_WaitEnterGame;
}

int CLoginPlayer::processWaitEnterGame()
{
	if (NULL == mClientMessage || ID_C2S_REQUEST_ENTER_SCENE != mClientMessageID)
	{
		return -1;
	}
	CMessageEnterSceneRequest* pEnterSceneRequest = reinterpret_cast<CMessageEnterSceneRequest*>(mClientMessage);
	if (NULL == pEnterSceneRequest)
	{
		return -1;
	}

	if (mServerID != pEnterSceneRequest->serverid()
		|| mChannelID != pEnterSceneRequest->channelid()
		|| mAccountID != pEnterSceneRequest->accountid())
	{
		return -1;
	}

	CIMEnterSceneRequest* pNewEnterSceneRequest = reinterpret_cast<CIMEnterSceneRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_ENTER_SCENE));
	if (NULL == pNewEnterSceneRequest)
	{
		return -1;
	}

	CEntityPlayer* pNewPlayer = reinterpret_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	if (NULL == pNewPlayer)
	{
		return -1;
	}

	pNewPlayer->setRoleID(pEnterSceneRequest->roleid());

	pNewEnterSceneRequest->mRoleID = pEnterSceneRequest->roleid();
	pNewEnterSceneRequest->mPlayerEntityID = pNewPlayer->getObjID();
	setPlayerLoginMsg(pNewEnterSceneRequest);

	CGameServer::Inst()->pushTask(emTaskType_DB, pNewEnterSceneRequest);

	return emLoginState_Playing;
}
int CLoginPlayer::processWaitPlaying()
{
	if (NULL == mDBMessage || IM_RESPONSE_ENTER_SCENE != mDBMessageID)
	{
		return -1;
	}

	CIMEnterSceneResponse* pEnterSceneResponse = reinterpret_cast<CIMEnterSceneResponse*>(mDBMessage);
	if (NULL == pEnterSceneResponse)
	{
		return -1;
	}

	if (mServerID != pEnterSceneResponse->mServerID
		|| mChannelID != pEnterSceneResponse->mChannelID
		|| mAccountID != pEnterSceneResponse->mAccountID)
	{
		return -1;
	}

	CMessageEnterSceneResponse tEnterSceneResponse;
	tEnterSceneResponse.set_result(0);
	CSceneJob::Inst()->sendClientMessage(mExchangeHead, ID_S2C_RESPONSE_CREATE_ROLE, &tEnterSceneResponse);

	return emLoginState_None;
}

void CLoginPlayer::setPlayerLoginMsg(CIMPlayerLoginMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	pMsg->mSocketIndex = mExchangeHead.mSocketIndex;
	pMsg->mAccountID = mAccountID;
	pMsg->mChannelID = mChannelID;
	pMsg->mServerID = mServerID;
}

bool CLoginPlayer::elapse(unsigned int nTickOffset)
{
	return mStateMachine.elapse(nTickOffset);
}