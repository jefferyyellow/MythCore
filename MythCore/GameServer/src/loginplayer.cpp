#include "loginplayer.h"
#include "loginmessage.hxx.pb.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "gameserver.h"

void CLoginPlayer::init()
{
	mStateMachine.addState(emLoginState_None,			10, &CLoginPlayer::processStateNone);
	mStateMachine.addState(emLoginState_AccountVerify,	10, &CLoginPlayer::processAccountVerify);
	mStateMachine.addState(emLoginState_WaitCreateRole, 10, &CLoginPlayer::processWaitCreateRole);
	mStateMachine.addState(emLoginState_CreateRoleing,	10, &CLoginPlayer::processCreateRoleing);
	mStateMachine.addState(emLoginState_WaitEnterGame,	10, &CLoginPlayer::processWaitEnterGame);
	mStateMachine.addState(emLoginState_Playing,		10, &CLoginPlayer::processWaitPlaying);
	mStateMachine.setCurState(emLoginState_None);
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
		return;
	}

	// ���յ��ͻ��˵���Ϣ��ת�����ڲ���Ϣ��DBȥ����
	CIMPlayerLoginRequest* pPlayerLoginRequest = reinterpret_cast<CIMPlayerLoginRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_PLAYER_LOGIN));
	if (NULL == pPlayerLoginRequest)
	{
		return;
	}

	// �û���
	strncpy(pPlayerLoginRequest->mName, pLoginRequest->name().c_str(), sizeof(pPlayerLoginRequest->mName));
	// ����ID
	pPlayerLoginRequest->mChannelID = pLoginRequest->channelid();
	// ������ID
	pPlayerLoginRequest->mServerID = pLoginRequest->serverid();
	// TCP��������Ϣͷ
	pPlayerLoginRequest->mExchangeHead = mExchangeHead;

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

	CIMPlayerLoginResponse* pResponse = reinterpret_cast<CIMPlayerLoginResponse*>(pMsg);


	uint64 nAccountID = pResponse->mAccountID;
	uint64 nChannelID = pResponse->mChannelID;
	uint64 nServerID = pResponse->mServerID;
	uint64 nKey = MAKE_LOGIN_KEY(nAccountID, nChannelID, nServerID);

	// ������Ѿ��ڵ�¼�б�����
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

	mLoginList[pLoginPlayer->getKey()] = pLoginPlayer->getObjID();

	// ׼����Ӧ��Ϣ
	CIMPlayerLoginResponse* pIMLoginResponse = reinterpret_cast<CIMPlayerLoginResponse*>(pMsg);
	CMessageLoginResponse tMessageLoginResponse;
	tMessageLoginResponse.set_accountid(pIMLoginResponse->mAccountID);
	tMessageLoginResponse.set_channelid(pIMLoginResponse->mChannelID);
	tMessageLoginResponse.set_serverid(pIMLoginResponse->mServerID);
	tMessageLoginResponse.set_roleid(pIMLoginResponse->mRoleID);

	CSceneJob::Inst()->sendClientMessage(mExchangeHead, ID_S2C_RESPONSE_LOGIN, &tMessageLoginResponse);

}

int CLoginPlayer::processWaitCreateRole()
{

}

int CLoginPlayer::processCreateRoleing()
{

}

int CLoginPlayer::processWaitEnterGame()
{

}
int CLoginPlayer::processWaitPlaying()
{

}