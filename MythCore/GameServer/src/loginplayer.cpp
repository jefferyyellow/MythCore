#include "loginplayer.h"
#include "loginmessage.hxx.pb.h"
#include "gameserver.h"
#include "objpool.h"
#include "entityplayer.h"
#include "dbmodule.h"

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

	setAccountName(pLoginRequest->name().c_str());
	setChannelID(pLoginRequest->channelid());
	setServerID(pLoginRequest->serverid());

	CDBModule::Inst()->pushDBTask(getObjID(), emSessionType_AccountVerify,0, 0, "call CheckUserName('%s', %d, %d)", 
		pLoginRequest->name().c_str(), pLoginRequest->channelid(), pLoginRequest->serverid());
	return emLoginState_AccountVerify;
}

int CLoginPlayer::processAccountVerify()
{
	if (NULL == mDBResponse || emSessionType_AccountVerify != mDBSessionType)
	{
		return -1;
	}

	// 准备回应消息
	mAccountID = mDBResponse->getInt();
	mRoleID = mDBResponse->getInt();

	CMessageLoginResponse tMessageLoginResponse;
	tMessageLoginResponse.set_accountid(mAccountID);
	tMessageLoginResponse.set_channelid(mChannelID);
	tMessageLoginResponse.set_serverid(mServerID);
	tMessageLoginResponse.set_roleid(mRoleID);

	CSceneJob::Inst()->sendClientMessage(mExchangeHead, ID_S2C_RESPONSE_LOGIN, &tMessageLoginResponse);
	if (mRoleID == 0)
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

	CDBModule::Inst()->pushDBTask(getObjID(), emSessionType_CreateRole, 0, 0, "call CreateRole(%d, '%s', %d, %d, %d)",
	pCreateRoleRequest->rolename().c_str(), mAccountID, mChannelID, mServerID);

	return emLoginState_CreateRoleing;
}

int CLoginPlayer::processCreateRoleing()
{
	if (NULL == mDBResponse || emSessionType_CreateRole != mDBSessionType)
	{
		return -1;
	}

	mRoleID = mDBResponse->getInt();

	CMessageCreateRoleResponse tCreateRoleResponse;
	tCreateRoleResponse.set_result(0);
	tCreateRoleResponse.set_roleid(mRoleID);
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



	CEntityPlayer* pNewPlayer = reinterpret_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	if (NULL == pNewPlayer)
	{
		return -1;
	}

	pNewPlayer->setRoleID(pEnterSceneRequest->roleid());
	CDBModule::Inst()->pushDBTask(getObjID(), emSessionType_LoadPlayerInfo, 0, 0, "call LoadPlayerInfo(%d)", mRoleID);
	return emLoginState_Playing;
}
int CLoginPlayer::processWaitPlaying()
{
	if (NULL == mDBResponse || emSessionType_LoadPlayerInfo != mDBSessionType)
	{
		return -1;
	}


	CMessageEnterSceneResponse tEnterSceneResponse;
	tEnterSceneResponse.set_result(0);
	CSceneJob::Inst()->sendClientMessage(mExchangeHead, ID_S2C_RESPONSE_ENTER_SCENE, &tEnterSceneResponse);

	return emLoginState_None;
}

bool CLoginPlayer::elapse(unsigned int nTickOffset)
{
	return mStateMachine.elapse(nTickOffset);
}