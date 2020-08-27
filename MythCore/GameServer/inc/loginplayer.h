#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__
#include "obj.h"
#include "statemachine.h"
#include "messagefactory.h"
#include "dbmessage.h"
class CDBResponse;
enum EmLoginState
{
	emLoginState_None				= 0,	// ��״̬
	emLoginState_AccountVerify		= 1,	// ��¼��֤
	emLoginState_WaitCreateRole		= 2,	// �ȴ�������ɫ
	emLoginState_CreateRoleing		= 3,	// ������ɫ��
	emLoginState_LoginComplete		= 4,	// ��¼���
	emLoginState_RoleLoading		= 5,	// ��ɫ���ݼ���
	//emLoginState_WaitEnterGame		= 4,	// �ȴ�������Ϸ
	//emLoginState_Playing			= 5,	// ��Ϸ״̬
	emLoginStateMax
};

// ��¼ɾ��״̬
enum EmLoginDelState
{
	emLoginDelState_None		= 0,		// ����ɾ��״̬
	emLoginDelState_Error		= 1,		// ����ɾ��
	emLoginDelState_Complete	= 2,		// ���ɾ��
};

class CLoginPlayer : public CObj
{
public:
	CLoginPlayer()
	{
		initStateMachine();
		init();
	}

	~CLoginPlayer(){}

public:
	void		init();
	void		initStateMachine();
	void		checkState();
	int			processStateNone();
	int			processAccountVerify();
	int			processWaitCreateRole();
	int			processCreateRoleing();
	int			processLoginComplete();
	int			processRoleLoading();
	int			onSwitchFailure();

	bool		elapse(unsigned int nTickOffset);
	void		setCurStateTime(int nTime);
	EmLoginState getLoginState(){return (EmLoginState)mStateMachine.getCurState();}
	void		setLoginState(int nState){mStateMachine.switchState(nState);}
	// �������ʵ�岢����
	int			createPlayerAndLoad();

public:
	/// autocode don't edit!!!
    unsigned int getAccountID(){ return mAccountID;}
    void setAccountID(unsigned int value){ mAccountID = value;}

    short getChannelID(){ return mChannelID;}
    void setChannelID(short value){ mChannelID = value;}

    short getServerID(){ return mServerID;}
    void setServerID(short value){ mServerID = value;}

    unsigned int getRoleID(){ return mRoleID;}
    void setRoleID(unsigned int value){ mRoleID = value;}

    char* getAccountName(){ return mAccountName;}
    void setAccountName(const char* value)
    {
        if (NULL == value)
        {
            return;
        }
        strncpy(mAccountName, value, sizeof(mAccountName) - 1);
    }

    CExchangeHead& getExchangeHead(){ return mExchangeHead;}

    Message* getClientMessage(){ return mClientMessage;}
    void setClientMessage(Message* value){ mClientMessage = value;}

    int getClientMessageID(){ return mClientMessageID;}
    void setClientMessageID(int value){ mClientMessageID = value;}

    CDBResponse* getDBResponse(){ return mDBResponse;}
    void setDBResponse(CDBResponse* value){ mDBResponse = value;}

    EmSessionType getDBSessionType(){ return mDBSessionType;}
    void setDBSessionType(EmSessionType value){ mDBSessionType = value;}

    EmLoginDelState getDelState(){ return mDelState;}
    void setDelState(EmLoginDelState value){ mDelState = value;}
	/// end autocode

private:
	unsigned int					mAccountID;								// �˺�ID
	short							mChannelID;								// ����
	short							mServerID;								// ������ID
	unsigned int					mRoleID;								// ��ɫID
	char							mAccountName[MAX_PLAYER_NAME_LEN + 1];	// �˺���
	CExchangeHead					mExchangeHead;							// TCP��Ϣ����ͷ
	/// default:ignore
	CStateMachine<CLoginPlayer, emLoginStateMax>	mStateMachine;			// ״̬��

	// �ӿͻ��˷���������Ϣ
	Message*						mClientMessage;
	// �ӿͻ��˷���������ϢID
	int								mClientMessageID;
	// ��DB����������Ϣ
	CDBResponse*					mDBResponse;
	// ��DB����������ϢID,default:emSessionType_None
	EmSessionType					mDBSessionType;
	// �Ƿ���ɾ��״̬(�Ѿ�������ߵ�¼���),default:emLoginDelState_None
	EmLoginDelState					mDelState;
};
#endif
