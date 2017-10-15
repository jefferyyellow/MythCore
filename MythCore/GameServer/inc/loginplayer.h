#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__
#include "obj.h"
#include "statemachine.h"
#include "messagefactory.h"

class CInternalMsg;
class CIMPlayerLoginMsg;
enum EMLoginState
{
	emLoginState_None				= 0,	// ��״̬
	emLoginState_AccountVerify		= 1,	// ��¼��֤
	emLoginState_WaitCreateRole		= 2,	// �ȴ�������ɫ
	emLoginState_CreateRoleing		= 3,	// ������ɫ��
	emLoginState_WaitEnterGame		= 4,	// �ȴ�������Ϸ
	emLoginState_Playing			= 5,	// ��Ϸ״̬
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
	int			processWaitEnterGame();
	int			processWaitPlaying();
	void		setPlayerLoginMsg(CIMPlayerLoginMsg* pMsg);
	bool		elapse(unsigned int nTickOffset);
public:
	uint32		getAccountID() const { return mAccountID; }
	void		setAccountID(uint32 nValue) { mAccountID = nValue; }

	uint16		getChannelID() const { return mChannelID; }
	void		setChannelID(uint16 nValue) { mChannelID = nValue; }

	uint16		getServerID() const { return mServerID; }
	void		setServerID(uint16 nValue) { mServerID = nValue; }

	uint32		getRoleID() const { return mRoleID; }
	void		setRoleID(uint32 nValue) { mRoleID = nValue; }

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

	CInternalMsg* getDBMessage() const { return mDBMessage; }
	void		setDBMessage(CInternalMsg* nValue) { mDBMessage = nValue; }

	int			getDBMessageID() const { return mDBMessageID; }
	void		setDBMessageID(int nValue) { mDBMessageID = nValue; }

private:
	uint32							mAccountID;								// �˺�ID
	uint16							mChannelID;								// ����
	uint16							mServerID;								// ������ID
	uint32							mRoleID;								// ��ɫID
	char							mAccountName[MAX_PLAYER_NAME_LEN];		// �˺���
	CExchangeHead					mExchangeHead;							// TCP��Ϣ����ͷ

	CStateMachine<CLoginPlayer, emLoginStateMax>	mStateMachine;			// ״̬��

	// �ӿͻ��˷���������Ϣ
	Message*						mClientMessage;
	// �ӿͻ��˷���������ϢID
	int								mClientMessageID;
	// ��DB����������Ϣ
	CInternalMsg*					mDBMessage;
	// ��DB����������ϢID
	int								mDBMessageID;
};
#endif
