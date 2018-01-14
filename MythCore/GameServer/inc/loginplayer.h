#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__
#include "obj.h"
#include "statemachine.h"
#include "messagefactory.h"
#include "dbmessage.h"
class CDBResponse;
enum EMLoginState
{
	emLoginState_None				= 0,	// ��״̬
	emLoginState_AccountVerify		= 1,	// ��¼��֤
	emLoginState_WaitCreateRole		= 2,	// �ȴ�������ɫ
	emLoginState_CreateRoleing		= 3,	// ������ɫ��
	emLoginState_LoginComplete		= 4,	// ��¼���
	//emLoginState_WaitEnterGame		= 4,	// �ȴ�������Ϸ
	//emLoginState_Playing			= 5,	// ��Ϸ״̬
	emLoginStateMax
};

class CLoginPlayer : public CObj
{
public:
	CLoginPlayer()
	{
		init();
	}

	~CLoginPlayer(){}

public:
	void		init();
	void		checkState();
	int			processStateNone();
	int			processAccountVerify();
	int			processWaitCreateRole();
	int			processCreateRoleing();
	bool		elapse(unsigned int nTickOffset);
	void		setCurStateTime(int nTime);
public:
	unsigned int getAccountID() const { return mAccountID; }
	void		setAccountID(int nValue) { mAccountID = nValue; }

	short		getChannelID() const { return mChannelID; }
	void		setChannelID(short nValue) { mChannelID = nValue; }

	short		getServerID() const { return mServerID; }
	void		setServerID(short nValue) { mServerID = nValue; }

	unsigned int getRoleID() const { return mRoleID; }
	void		setRoleID(unsigned int nValue) { mRoleID = nValue; }

	void		setAccountName(const char* pName)
	{
		if (NULL == pName)
		{
			return;
		}
		strncpy(mAccountName, pName, sizeof(mAccountName) - 1);
	}
	char*		getAccountName(){return mAccountName;}

	CExchangeHead&	getExchangeHead(){ return mExchangeHead; }

	Message*	getClientMessage() const { return mClientMessage; }
	void		setClientMessage(Message* nValue) { mClientMessage = nValue; }

	int			getClientMessageID() const { return mClientMessageID; }
	void		setClientMessageID(int nValue) { mClientMessageID = nValue; }

	CDBResponse* getDBMessage() const { return mDBResponse; }
	void		setDBMessage(CDBResponse* nValue) { mDBResponse = nValue; }

	EmSessionType getDBSessionType() const { return mDBSessionType; }
	void		setDBSessionType(EmSessionType eValue) { mDBSessionType = eValue; }

	EMLoginState getLoginState(){return (EMLoginState)mStateMachine.getCurState();}
private:
	unsigned int					mAccountID;								// �˺�ID
	short							mChannelID;								// ����
	short							mServerID;								// ������ID
	unsigned int					mRoleID;								// ��ɫID
	char							mAccountName[MAX_PLAYER_NAME_LEN + 1];		// �˺���
	CExchangeHead					mExchangeHead;							// TCP��Ϣ����ͷ

	CStateMachine<CLoginPlayer, emLoginStateMax>	mStateMachine;			// ״̬��

	// �ӿͻ��˷���������Ϣ
	Message*						mClientMessage;
	// �ӿͻ��˷���������ϢID
	int								mClientMessageID;
	// ��DB����������Ϣ
	CDBResponse*					mDBResponse;
	// ��DB����������ϢID
	EmSessionType					mDBSessionType;
};
#endif
