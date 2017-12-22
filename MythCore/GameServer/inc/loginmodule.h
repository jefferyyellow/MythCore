#ifndef __LOGINMODULE_H__
#define __LOGINMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
#include "commontype.h"
#include "logintype.h"
#include "logicmodule.h"
#include "timemanager.h"
#include <map>

using namespace Myth;
class CDBResponse;
class CLoginPlayer;
#define  MAKE_LOGIN_KEY(AccountID, ChannelID, ServerID) ( (ChannelID << 48) | (ServerID << 32) | AccountID)
class CLoginModule : public CLogicModule, public CSingleton < CLoginModule >
{
	friend class CSingleton < CLoginModule > ;
public:
	typedef std::map<short, int> LOGIN_LIST;
private:
	CLoginModule();
	~CLoginModule();

public:
	/// 时间函数
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	/// 处理客户端消息
	void		onClientMessage(CExchangeHead& rExchangeHead, unsigned int nMessageID, Message* pMessage);
	/// 处理数据库消息
	void		OnDBMessage(CDBResponse* pMsg);
	/// 处理等待玩家进入游戏
	void		processWaitEnterGame(CLoginPlayer* pLoginPlayer, Message* pMessage);
	///  一个Socket断开
	void		onSocketDisconnect(int nSocketIndex);
private:
	LOGIN_LIST			mLoginList;				// 登录列表
	CAutoResetTimer		mLoginCheckTime;		// 登录校验时间
};
#endif