#include "loginplayer.h"
#include "loginmessage.hxx.pb.h"
#include "gameserver.h"
#include "objpool.h"
#include "entityplayer.h"
#include "dbmodule.h"

void CLoginPlayer::init()
{
	mStateMachine.init(this, emLoginState_None);
	mStateMachine.addState(emLoginState_None,			5, &CLoginPlayer::processStateNone);
	mStateMachine.addState(emLoginState_AccountVerify,	5, &CLoginPlayer::processAccountVerify);
	mStateMachine.addState(emLoginState_WaitCreateRole, 10, &CLoginPlayer::processWaitCreateRole);
	mStateMachine.addState(emLoginState_CreateRoleing,	5, &CLoginPlayer::processCreateRoleing);
	//mStateMachine.addState(emLoginState_WaitEnterGame,	10, &CLoginPlayer::processWaitEnterGame);
	//mStateMachine.addState(emLoginState_Playing,		10, &CLoginPlayer::processWaitPlaying);
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

	CLoginRequest* pLoginRequest = reinterpret_cast<CLoginRequest*>(mClientMessage);
	if (NULL == pLoginRequest)
	{
		return -1;
	}
	printf("\nCLoginPlayer::processStateNone\n");

	setAccountName(pLoginRequest->name().c_str());
	setChannelID(pLoginRequest->channelid());
	setServerID(pLoginRequest->serverid());

	CDBModule::Inst()->pushDBTask(0, emSessionType_AccountVerify,getObjID(), 0, "call CheckUserName('%s', %d, %d)", 
		pLoginRequest->name().c_str(), pLoginRequest->channelid(), pLoginRequest->serverid());
	return emLoginState_AccountVerify;
}

int CLoginPlayer::processAccountVerify()
{
	printf("\nprocessStateNone\n");

	if (NULL == mDBResponse || emSessionType_AccountVerify != mDBSessionType)
	{
		return -1;
	}

	// 准备回应消息
	mAccountID = mDBResponse->getInt();
	mRoleID = mDBResponse->getInt();

	CLoginResponse tLoginResponse;
	tLoginResponse.set_accountid(mAccountID);
	tLoginResponse.set_channelid(mChannelID);
	tLoginResponse.set_serverid(mServerID);
	tLoginResponse.set_roleid(mRoleID);
	printf("\nCLoginPlayer::processAccountVerify\n");
	CSceneJob::Inst()->send2Player(mExchangeHead, ID_S2C_RESPONSE_LOGIN, &tLoginResponse);
	if (mRoleID == 0)
	{
		return emLoginState_WaitCreateRole;
	}
	else
	{
		return emLoginState_LoginComplete;
	}
}

int CLoginPlayer::processWaitCreateRole()
{
	if (NULL == mClientMessage || ID_C2S_REQUEST_CREATE_ROLE != mClientMessageID)
	{
		return -1;
	}
	CCreateRoleRequest* pCreateRoleRequest = reinterpret_cast<CCreateRoleRequest*>(mClientMessage);
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

	CDBModule::Inst()->pushDBTask(0, emSessionType_CreateRole, getObjID(), 0, "call CreateRole(%d, '%s', %d, %d, %d)",
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

	CCreateRoleResponse tCreateRoleResponse;
	tCreateRoleResponse.set_result(0);
	tCreateRoleResponse.set_roleid(mRoleID);
	CSceneJob::Inst()->send2Player(mExchangeHead, ID_S2C_RESPONSE_CREATE_ROLE, &tCreateRoleResponse);

	return emLoginState_LoginComplete;
}

bool CLoginPlayer::elapse(unsigned int nTickOffset)
{
	return mStateMachine.elapse(nTickOffset);
}