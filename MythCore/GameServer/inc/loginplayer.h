#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__
#include "obj.h"
#include "statemachine.h"
#include "messagefactory.h"

class CInternalMsg;
class CIMPlayerLoginMsg;
enum EMLoginState
{
	emLoginState_None				= 0,	// 无状态
	emLoginState_AccountVerify		= 1,	// 登录验证
	emLoginState_WaitCreateRole		= 2,	// 等待创建角色
	emLoginState_CreateRoleing		= 3,	// 创建角色中
	emLoginState_WaitEnterGame		= 4,	// 等待进入游戏
	emLoginState_Playing			= 5,	// 游戏状态
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

	CInternalMsg* getDBMessage() const { return mDBMessage; }
	void		setDBMessage(CInternalMsg* nValue) { mDBMessage = nValue; }

	int			getDBMessageID() const { return mDBMessageID; }
	void		setDBMessageID(int nValue) { mDBMessageID = nValue; }

private:
	unsigned int					mAccountID;								// 账号ID
	short							mChannelID;								// 渠道
	short							mServerID;								// 服务器ID
	unsigned int					mRoleID;								// 角色ID
	char							mAccountName[MAX_PLAYER_NAME_LEN];		// 账号名
	CExchangeHead					mExchangeHead;							// TCP消息交换头

	CStateMachine<CLoginPlayer, emLoginStateMax>	mStateMachine;			// 状态机

	// 从客户端发过来的消息
	Message*						mClientMessage;
	// 从客户端发过来的消息ID
	int								mClientMessageID;
	// 从DB发过来的消息
	CInternalMsg*					mDBMessage;
	// 从DB发过来的消息ID
	int								mDBMessageID;
};
#endif
