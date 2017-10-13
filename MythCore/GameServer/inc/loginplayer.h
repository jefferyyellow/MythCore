#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__
#include "obj.h"
#include "statemachine.h"
class Message;
class CInternalMsg;
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

public:
	uint64		getKey(){ return mKey; }
	void		setKey(uint64 nKey){ mKey = nKey; }

	uint32		getRoleID() const { return mRoleID; }
	void		setRoleID(uint32 nValue) { mRoleID = nValue; }

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
	uint64							mKey;
	uint32							mRoleID;
	char							mName[MAX_PLAYER_NAME_LEN];
	CExchangeHead					mExchangeHead;

	CStateMachine<CLoginPlayer, emLoginStateMax>	mStateMachine;

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
