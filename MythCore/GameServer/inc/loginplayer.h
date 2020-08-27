#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__
#include "obj.h"
#include "statemachine.h"
#include "messagefactory.h"
#include "dbmessage.h"
class CDBResponse;
enum EmLoginState
{
	emLoginState_None				= 0,	// 无状态
	emLoginState_AccountVerify		= 1,	// 登录验证
	emLoginState_WaitCreateRole		= 2,	// 等待创建角色
	emLoginState_CreateRoleing		= 3,	// 创建角色中
	emLoginState_LoginComplete		= 4,	// 登录完成
	emLoginState_RoleLoading		= 5,	// 角色数据加载
	//emLoginState_WaitEnterGame		= 4,	// 等待进入游戏
	//emLoginState_Playing			= 5,	// 游戏状态
	emLoginStateMax
};

// 登录删除状态
enum EmLoginDelState
{
	emLoginDelState_None		= 0,		// 不是删除状态
	emLoginDelState_Error		= 1,		// 出错删除
	emLoginDelState_Complete	= 2,		// 完成删除
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
	// 创建玩家实体并加载
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
	unsigned int					mAccountID;								// 账号ID
	short							mChannelID;								// 渠道
	short							mServerID;								// 服务器ID
	unsigned int					mRoleID;								// 角色ID
	char							mAccountName[MAX_PLAYER_NAME_LEN + 1];	// 账号名
	CExchangeHead					mExchangeHead;							// TCP消息交换头
	/// default:ignore
	CStateMachine<CLoginPlayer, emLoginStateMax>	mStateMachine;			// 状态机

	// 从客户端发过来的消息
	Message*						mClientMessage;
	// 从客户端发过来的消息ID
	int								mClientMessageID;
	// 从DB发过来的消息
	CDBResponse*					mDBResponse;
	// 从DB发过来的消息ID,default:emSessionType_None
	EmSessionType					mDBSessionType;
	// 是否是删除状态(已经出错或者登录完成),default:emLoginDelState_None
	EmLoginDelState					mDelState;
};
#endif
