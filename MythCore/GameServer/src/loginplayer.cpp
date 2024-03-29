#include "loginplayer.h"
#include "loginmessage.hxx.pb.h"
#include "gameserver.h"
#include "objpool.h"
#include "entityplayer.h"
#include "dbmodule.h"
#include "errcode.h"
#include "locallogjob.h"
#include "dirtyword.h"
#include "i18n.h"
#include "crc32.h"
#include "propertymodule.h"
#include "clientsocketjob.h"
#include "message.hxx.pb.h"
void CLoginPlayer::init()
{
    mAccountID = 0;
    mChannelID = 0;
    mServerID = 0;
    mRoleID = 0;
    mAccountName[0] = '\0';
    mClientMessage = NULL;
    mClientMessageID = 0;
    mDBResponse = NULL;
    mDBSessionType = emSessionType_None;
    mDelState = emLoginDelState_None;
}

void CLoginPlayer::initStateMachine()
{
	mStateMachine.init(this, emLoginState_None);
	mStateMachine.addState(emLoginState_None,			1, &CLoginPlayer::processStateNone);
	mStateMachine.addState(emLoginState_AccountVerify,	5, &CLoginPlayer::processAccountVerify);
	mStateMachine.addState(emLoginState_WaitCreateRole, 120, &CLoginPlayer::processWaitCreateRole);
	mStateMachine.addState(emLoginState_CreateRoleing,	5, &CLoginPlayer::processCreateRoleing);
	mStateMachine.addState(emLoginState_LoginComplete, 5, &CLoginPlayer::processLoginComplete);
	mStateMachine.addState(emLoginState_RoleLoading, 15, &CLoginPlayer::processRoleLoading);
	mStateMachine.setSwitchFailure(&CLoginPlayer::onSwitchFailure);
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
		return -1;
	}

	CLoginRequest* pLoginRequest = static_cast<CLoginRequest*>(mClientMessage);
	if (NULL == pLoginRequest)
	{
		// 将当前的状态时间置零
		LOG_ERROR("CLoginRequest message is null!");
		return -1;
	}

	setAccountName(pLoginRequest->name().c_str());
	setChannelID(pLoginRequest->channelid());
	setServerID(pLoginRequest->serverid());
	LOG_INFO("player login, AccountName:%s, ChannelID:%d, ServerID:%d",
		mAccountName, mChannelID, mServerID);

	// 电脑直接登录的,计算crc32码
	unsigned char* pAccountName = (unsigned char*)mAccountName;
	unsigned int nAccountID = crc32(pAccountName, strlen(mAccountName));

	CDBModule::Inst()->pushDBTask(getJobID(), 0, emSessionType_AccountVerify,getObjID(), 0, "call CheckUserName('%s', %u, %d, %d)", 
		pLoginRequest->name().c_str(), nAccountID, pLoginRequest->channelid(), pLoginRequest->serverid());
	return emLoginState_AccountVerify;
}

int CLoginPlayer::processAccountVerify()
{
	if (NULL == mDBResponse || emSessionType_AccountVerify != mDBSessionType)
	{
		// 将当前的状态时间置零
		LOG_ERROR("processAccountVerify, Account Name: %s, ChannelId: %d, ServerId: %d, DB Session Type: %d", mAccountName, mChannelID, mServerID, mDBSessionType);
		return -1;
	}
	if (SUCCESS != mDBResponse->mSqlResult)
	{
		LOG_ERROR("processAccountVerify, Account Name: %s, ChannelId: %d, ServerId: %d, DB Result: %d", mAccountName, mChannelID, mServerID, mDBResponse->mSqlResult);
		return -1;
	}

	// 准备回应消息
	char acName[MAX_PLAYER_NAME_LEN + 1] = {0};
	mDBResponse->getString(acName, sizeof(acName));
	if (0 != strncmp(acName, mAccountName, MAX_PLAYER_NAME_LEN))
	{
		LOG_ERROR("Account Name is not match, Account Name: %s, AccountId: %d, ChannelId: %d, ServerId: %d, Another Name: %s", mAccountName, mAccountID, mChannelID, mServerID, acName);
		return -1;
	}


	mAccountID = (unsigned int)mDBResponse->getUInt();
	// 账号没有插入成功
	if (0 == mAccountID)
	{
		LOG_ERROR("Account id is zero, Account Name: %s, AccountId: %d, ChannelId: %d, ServerId: %d", mAccountName, mAccountID, mChannelID, mServerID);
		return -1;
	}

	// 玩家已经在校验当中了
	bool bCheckVerify = CLoginModule::Inst()->checkVerifyPlayer(mChannelID, mServerID, mAccountID);
	if (bCheckVerify)
	{
		LOG_ERROR("Player already verify, AccountName: %s, ChannelID: %d, ServerID: %d, AccountID: %d",
			mAccountName, mChannelID, mServerID, mAccountID);
		return -1;
	}

	bool bAdd = CLoginModule::Inst()->addVerifyPlayer(mChannelID, mServerID, mAccountID, getObjID());
	if (!bAdd)
	{
		LOG_ERROR("Add player to verify list failure, AccountName: %s, ChannelID: %d, ServerID: %d, AccountID: %d",
			mAccountName, mChannelID, mServerID, mAccountID);
		return -1;
	}
	mRoleID = mDBResponse->getInt();

	CLoginResponse tLoginResponse;
	tLoginResponse.set_accountid(mAccountID);
	tLoginResponse.set_channelid(mChannelID);
	tLoginResponse.set_serverid(mServerID);
	tLoginResponse.set_roleid(mRoleID);
	
	//CSceneJob::Inst()->send2Player(this, ID_S2C_RESPONSE_LOGIN, &tLoginResponse);
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
		return -1;
	}
	CCreateRoleRequest* pCreateRoleRequest = static_cast<CCreateRoleRequest*>(mClientMessage);
	if (NULL == pCreateRoleRequest)
	{
		LOG_ERROR("Create role request is null!");
		// 将当前的状态时间置零
		return -1;
	}

	if (mServerID != pCreateRoleRequest->serverid()
		|| mChannelID != pCreateRoleRequest->channelid()
		|| mAccountID != pCreateRoleRequest->accountid())
	{
		// 将当前的状态时间置零
		LOG_ERROR("Server id, channel id, account id is not match, old server id: %d, channel id: %d, account id: %d,request server id: %d, channel id: %d, account id: %d",
			mServerID, mChannelID, mAccountID, pCreateRoleRequest->serverid(), pCreateRoleRequest->channelid(), pCreateRoleRequest->accountid());
		return -1;
	}

	const char* pRoleName = pCreateRoleRequest->rolename().c_str();
	if (NULL == pRoleName)
	{
		return -1;
	}

	EmPlayerSex eSex = (EmPlayerSex)pCreateRoleRequest->sex();
	int nMetier = pCreateRoleRequest->metier();

	wchar_t acBuffer[MAX_PLAYER_NAME_LEN * 2] = {0};
	CI18N::AnsiToUnicode(pRoleName, acBuffer, sizeof(acBuffer) - 1);

	// 名字长度
	int nNameLength = wcslen(acBuffer);
	if (nNameLength < MIN_PLAYER_NAME_CHAR)
	{
		return emLoginState_WaitCreateRole;
	}

	// 名字长度
	if (nNameLength > MAX_PLAYER_NAME_LEN - PLAYER_NAME_RESERVE || nNameLength > MAX_PLAYER_NAME_CHAR)
	{
		return emLoginState_WaitCreateRole;
	}

	// 检查脏字
	if (CDirtyWord::Inst()->checkDirtyWord(pRoleName))
	{
		return emLoginState_WaitCreateRole;
	}

	if (eSex < 0 || eSex >= EmPlayerSex_None)
	{
		return -1;
	}

	// 判断职业是否正确


	unsigned int nRoleID = CLoginModule::Inst()->allocateRoleID(mServerID);
	if (nRoleID <= 0)
	{
		LOG_ERROR("allocate role id invalid, role id: %d", nRoleID);
		return -1;
	}

	LOG_INFO("Create role Accountid: %d, ChannelId: %d, ServerId: %d", mAccountID, mChannelID, mServerID);
	CDBModule::Inst()->pushDBTask(getJobID(), 0, emSessionType_CreateRole, getObjID(), 0, "call CreateRole(%u, '%s', %d, %d, '%s', %u, %d, %d)",
	nRoleID, pCreateRoleRequest->rolename().c_str(), eSex, nMetier, getAccountName(), mAccountID, mChannelID, mServerID);

	return emLoginState_CreateRoleing;
}

int CLoginPlayer::processCreateRoleing()
{
	if (NULL == mDBResponse || emSessionType_CreateRole != mDBSessionType)
	{
		LOG_ERROR("DBResponse is null or session type is create role!");
		// 将当前的状态时间置零
		return -1;
	}
	
	if (SUCCESS != mDBResponse->mSqlResult)
	{
		LOG_ERROR("mysql create role error: %d", mDBResponse->mSqlResult);
		return -1;
	}
	int nResult = mDBResponse->getInt();
	if (nResult < 0)
	{
		LOG_ERROR("create role failure, nResult: %d, RoleID:%d, AccountName:%s, AccountID:%d, ChannelID:%d, ServerID:%d",
			nResult, mRoleID, mAccountName, mAccountID, mChannelID, mServerID);
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
	//CSceneJob::Inst()->send2Player(this, ID_S2C_RESPONSE_CREATE_ROLE, &tCreateRoleResponse);

	return emLoginState_LoginComplete;
}

int CLoginPlayer::processLoginComplete()
{
	if (NULL == mClientMessage || ID_C2S_REQUEST_ENTER_SCENE != mClientMessageID)
	{
		LOG_ERROR("Client message is null or client message id not create role!");
		// 将当前的状态时间置零
		return -1;
	}

	CEnterSceneRequest* pEnterSceneRequest = static_cast<CEnterSceneRequest*>(mClientMessage);
	if (NULL == pEnterSceneRequest)
	{
		LOG_ERROR("Create role request is null!");
		// 将当前的状态时间置零
		return -1;
	}
	if (getServerID() != pEnterSceneRequest->serverid()
		|| getChannelID() != pEnterSceneRequest->channelid()
		|| getAccountID() != pEnterSceneRequest->accountid()
		|| getRoleID() != pEnterSceneRequest->roleid())
	{
		return -1;
	}

	if (mRoleID != pEnterSceneRequest->roleid())
	{
		LOG_ERROR("enter scene role id invalid: %d", pEnterSceneRequest->roleid());
		return -1;
	}

	//CEntityPlayer* pPlayer = CSceneJob::Inst()->getPlayerByRoleID(pEnterSceneRequest->roleid());
	//if (NULL != pPlayer)
	//{
	//	printf("Kick out by other: %d\n", pEnterSceneRequest->roleid());
	//	// 如果加入不了到列表，就踢不下线
	//	if (!CSceneJob::Inst()->addPlayerSocketIndex(getExchangeHead().mSocketIndex, pPlayer->getObjID()))
	//	{
	//		CSceneJob::Inst()->disconnectPlayer(getExchangeHead());
	//		return -1;
	//	}
	//	// 玩家都在游戏中活着退出过程中，表示玩家数据是完整的，可以直接用
	//	if (pPlayer->getPlayerStauts() == emPlayerStatus_Gameing || pPlayer->getPlayerStauts() == emPlayerStatus_Exiting)
	//	{
	//		pPlayer->setPlayerStauts(emPlayerStatus_Gameing);

	//		// 将原来的玩家下线
	//		CSceneJob::Inst()->disconnectPlayer(*pPlayer);

	//		// 换成新的玩家socket信息
	//		pPlayer->getExchangeHead() = getExchangeHead();
	//		CEnterSceneResponse tEnterSceneResponse;
	//		tEnterSceneResponse.set_result(0);
	//		CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_RESPONSE_ENTER_SCENE, &tEnterSceneResponse);
	//		setDelState(emLoginDelState_Complete);
	//		return emLoginState_None;
	//	}
	//	else
	//	{
	//		// 将玩家销毁
	//		CPropertyModule::Inst()->destroyPlayer(*pPlayer);
	//		return createPlayerAndLoad();
	//	}
	//}
	//else
	//{
	//	return createPlayerAndLoad();
	//}
}

// 创建玩家实体并加载
int CLoginPlayer::createPlayerAndLoad()
{
	printf("new player login: %d\n", getRoleID());
	CEntityPlayer* pNewPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	if (NULL == pNewPlayer)
	{
		return -1;
	}

	pNewPlayer->setPlayerStauts(emPlayerStatus_Loading);
	pNewPlayer->setRoleID(getRoleID());
	pNewPlayer->getExchangeHead() = getExchangeHead();

	//bool bResult = CSceneJob::Inst()->onPlayerLogin(pNewPlayer);
	//if (!bResult)
	//{
	//	CObjPool::Inst()->free(pNewPlayer->getObjID());
	//	return -1;
	//}
	printf("******processWaitEnterGame: %d\n", pNewPlayer->getObjID());

	pNewPlayer->setLoadStatus(0);
	CDBModule::Inst()->pushDBTask(getJobID(), getRoleID(), emSessionType_LoadPlayerInfo, pNewPlayer->getObjID(), 0, "call LoadPlayerInfo(%u)", getRoleID());
	CDBModule::Inst()->pushDBTask(getJobID(), getRoleID(), emSessionType_LoadPlayerBaseProperty, pNewPlayer->getObjID(), 0, "call LoadPlayerBaseProperty(%u)", getRoleID());

	CEnterSceneResponse tEnterSceneResponse;
	tEnterSceneResponse.set_result(0);
	//CSceneJob::Inst()->send2Player(pNewPlayer, ID_S2C_RESPONSE_ENTER_SCENE, &tEnterSceneResponse);
	return emLoginState_RoleLoading;
}

int CLoginPlayer::processRoleLoading()
{
	// 返回本来状态
	return emLoginState_RoleLoading;
}

bool CLoginPlayer::elapse(unsigned int nTickOffset)
{
	return mStateMachine.elapse(nTickOffset);
}

void CLoginPlayer::setCurStateTime(int nTime)
{
	mStateMachine.setTime(nTime);
}

int CLoginPlayer::onSwitchFailure()
{
	setDelState(emLoginDelState_Error);
	return 0;
}

// 时间函数
void CLoginPlayer::onTimer()
{
	if (NULL == mMsgQueue)
	{
		return;
	}
	CByteStream& rByteStream = mMsgQueue->mReceiveStream;
	char mBuffer[MAX_PACKAGE_SIZE + sizeof(CExchangeHead)];
	int nMessageLen = MAX_PACKAGE_SIZE;

	int nResult = rByteStream.GetHeadPacket((byte*)mBuffer, nMessageLen);
	if (nResult < 0)
	{
		return;
	}

	if (nMessageLen <= 0)
	{
		return;
	}

	char* pTemp = mBuffer;
	short nLength = *(short*)pTemp;
	if (nLength != nMessageLen)
	{
		return;
	}

	pTemp += sizeof(short);
	nMessageLen -= sizeof(short);

	short nMessageID = *(short*)pTemp;
	pTemp += sizeof(short);
	nMessageLen -= sizeof(short);

	Message* pMessage = CMessageFactory::Inst()->createClientMessage(nMessageID);
	if (NULL != pMessage)
	{
		printf("Recive MessageID: %d\n", nMessageID);
		pMessage->ParseFromArray(pTemp, nMessageLen);
		int nModule = nMessageID & MESSAGE_MODULE_MASK;
		if (nModule == MESSAGE_MODULE_LOGIN)
		{
			setClientMessage(pMessage);
			setClientMessageID(nMessageID);
			checkState();
			setClientMessage(NULL);
			setClientMessageID(0);

		}
		const ::google::protobuf::Descriptor* pDescriptor = pMessage->GetDescriptor();

		LOG_DEBUG("default", "---- Receive from client(Obj Id:%d|Account Id:%d|Channel:%d|Server:%d|Role:%d|Name:%s) \
							 Msg[ %s ][id: 0x%04x/%d] ---",
							 getObjID(), getAccountID(), getChannelID(),
							 getServerID(), getRoleID(), getAccountName(),
							 pDescriptor->name().c_str(), nMessageID, nMessageID);
		LOG_DEBUG("default", "[%s]", pMessage->ShortDebugString().c_str());

		pMessage->~Message();
	}
}