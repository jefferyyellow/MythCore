#include "loginplayer.h"
#include "loginmessage.hxx.pb.h"
#include "gameserver.h"
#include "objpool.h"
#include "entityplayer.h"
#include "dbmodule.h"
#include "errcode.h"
#include "locallogjob.h"
void CLoginPlayer::init()
{
	mStateMachine.init(this, emLoginState_None);
	mStateMachine.addState(emLoginState_None,			5, &CLoginPlayer::processStateNone);
	mStateMachine.addState(emLoginState_AccountVerify,	5, &CLoginPlayer::processAccountVerify);
	mStateMachine.addState(emLoginState_WaitCreateRole, 10, &CLoginPlayer::processWaitCreateRole);
	mStateMachine.addState(emLoginState_CreateRoleing,	5, &CLoginPlayer::processCreateRoleing);
	mStateMachine.addState(emLoginState_LoginComplete, 5, NULL);

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
		// 将当前的状态时间置零
		LOG_ERROR("Client message is null or client message id is login request!");
		setCurStateTime(0);
		return -1;
	}

	CLoginRequest* pLoginRequest = static_cast<CLoginRequest*>(mClientMessage);
	if (NULL == pLoginRequest)
	{
		// 将当前的状态时间置零
		LOG_ERROR("CLoginRequest message is null!");
		setCurStateTime(0);
		return -1;
	}

	setAccountName(pLoginRequest->name().c_str());
	setChannelID(pLoginRequest->channelid());
	setServerID(pLoginRequest->serverid());
	LOG_INFO("player login, AccountName:%s, ChannelID:%d, ServerID:%d",
		mAccountName, mChannelID, mServerID);

	CDBModule::Inst()->pushDBTask(0, emSessionType_AccountVerify,getObjID(), 0, "call CheckUserName('%s', %d, %d)", 
		pLoginRequest->name().c_str(), pLoginRequest->channelid(), pLoginRequest->serverid());
	return emLoginState_AccountVerify;
}

int CLoginPlayer::processAccountVerify()
{
	if (NULL == mDBResponse || emSessionType_AccountVerify != mDBSessionType)
	{
		// 将当前的状态时间置零
		setCurStateTime(0);
		LOG_ERROR("processAccountVerify, Account Name: %s, ChannelId: %d, ServerId: %d, DB Session Type: %d", mAccountName, mChannelID, mServerID, mDBSessionType);
		return -1;
	}
	if (SUCCESS != mDBResponse->mResult)
	{
		setCurStateTime(0);
		LOG_ERROR("processAccountVerify, Account Name: %s, ChannelId: %d, ServerId: %d, DB Result: %d", mAccountName, mChannelID, mServerID, mDBResponse->mResult);
		return -1;
	}

	// 准备回应消息
	char acName[MAX_PLAYER_NAME_LEN + 1] = {0};
	mDBResponse->getString(acName, sizeof(acName));
	if (0 != strncmp(acName, mAccountName, MAX_PLAYER_NAME_LEN))
	{
		LOG_ERROR("Account Name is not match, Account Name: %s, AccountId: %d, ChannelId: %d, ServerId: %d, Another Name: %s", mAccountName, mAccountID, mChannelID, mServerID, acName);
		setCurStateTime(0);
		return -1;
	}


	mAccountID = mDBResponse->getInt();
	// 账号没有插入成功
	if (0 == mAccountID)
	{
		LOG_ERROR("Account id is zero, Account Name: %s, AccountId: %d, ChannelId: %d, ServerId: %d", mAccountName, mAccountID, mChannelID, mServerID);
		setCurStateTime(0);
		return -1;
	}

	// 玩家已经在校验当中了
	bool bCheckVerify = CLoginModule::Inst()->checkVerifyPlayer(mChannelID, mServerID, mAccountID);
	if (bCheckVerify)
	{
		LOG_ERROR("Player already verify, AccountName: %s, ChannelID: %d, ServerID: %d, AccountID: %d",
			mAccountName, mChannelID, mServerID, mAccountID);
		setCurStateTime(0);
		return -1;
	}

	bool bAdd = CLoginModule::Inst()->addVerifyPlayer(mChannelID, mServerID, mAccountID, getObjID());
	if (!bAdd)
	{
		LOG_ERROR("Add player to verify list failure, AccountName: %s, ChannelID: %d, ServerID: %d, AccountID: %d",
			mAccountName, mChannelID, mServerID, mAccountID);
		setCurStateTime(0);
		return -1;
	}
	mRoleID = mDBResponse->getInt();

	CLoginResponse tLoginResponse;
	tLoginResponse.set_accountid(mAccountID);
	tLoginResponse.set_channelid(mChannelID);
	tLoginResponse.set_serverid(mServerID);
	tLoginResponse.set_roleid(mRoleID);
	
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
		LOG_ERROR("Client message is null or client message id not create role!");
		// 将当前的状态时间置零
		setCurStateTime(0);
		return -1;
	}
	CCreateRoleRequest* pCreateRoleRequest = static_cast<CCreateRoleRequest*>(mClientMessage);
	if (NULL == pCreateRoleRequest)
	{
		LOG_ERROR("Create role request is null!");
		// 将当前的状态时间置零
		setCurStateTime(0);
		return -1;
	}

	if (mServerID != pCreateRoleRequest->serverid()
		|| mChannelID != pCreateRoleRequest->channelid()
		|| mAccountID != pCreateRoleRequest->accountid())
	{
		// 将当前的状态时间置零
		LOG_ERROR("Server id, channel id, account id is not match, old server id: %d, channel id: %d, account id: %d,request server id: %d, channel id: %d, account id: %d",
			mServerID, mChannelID, mAccountID, pCreateRoleRequest->serverid(), pCreateRoleRequest->channelid(), pCreateRoleRequest->accountid());
		setCurStateTime(0);
		return -1;
	}

	int nRoleID = CLoginModule::Inst()->allocateRoleID(mServerID);
	if (nRoleID <= 0)
	{
		setCurStateTime(0);
		LOG_ERROR("allocate role id invalid, role id: %d", nRoleID);
		return -1;
	}

	LOG_INFO("Create role Accountid: %d, ChannelId: %d, ServerId: %d", mAccountID, mChannelID, mServerID);
	CDBModule::Inst()->pushDBTask(0, emSessionType_CreateRole, getObjID(), 0, "call CreateRole(%d, '%s', %d, %d, %d)",
	nRoleID, pCreateRoleRequest->rolename().c_str(), mAccountID, mChannelID, mServerID);

	return emLoginState_CreateRoleing;
}

int CLoginPlayer::processCreateRoleing()
{
	if (NULL == mDBResponse || emSessionType_CreateRole != mDBSessionType)
	{
		LOG_ERROR("DBResponse is null or session type is create role!");
		// 将当前的状态时间置零
		setCurStateTime(0);
		return -1;
	}
	
	if (SUCCESS != mDBResponse->mResult)
	{
		LOG_ERROR("mysql create role error: %d", mDBResponse->mResult);
		return -1;
	}
	mRoleID = mDBResponse->getInt();
	if (mRoleID <= 0)
	{
		LOG_ERROR("create role failure, RoleID:%d, AccountName:%s, AccountID:%d, ChannelID:%d, ServerID:%d",
			mRoleID, mAccountName, mAccountID, mChannelID, mServerID);
		return -1;
	}

	LOG_INFO("Create role complete, Accountid: %d, ChannelId: %d, ServerId: %d, RoleId: %d", mAccountID, mChannelID, mServerID, mRoleID);
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

void CLoginPlayer::setCurStateTime(int nTime)
{
	mStateMachine.setTime(nTime);
}
