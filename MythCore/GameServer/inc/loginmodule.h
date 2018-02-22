#ifndef __LOGINMODULE_H__
#define __LOGINMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
#include "servercommon.h"
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
	/// 启动服务器
	virtual void onLaunchServer();
	/// 启动完成检查
	virtual bool onCheckLaunch();
	/// 服务器启动成功
	virtual void onLaunchComplete();
	/// 退出服务器
	virtual void onExitServer();
	/// 新一天到来
	virtual void onNewDayCome();
	/// 新一周到来
	virtual void onNewWeekCome();
	/// 建立实体
	virtual void onCreatePlayer(CEntityPlayer* pPlayer);
	/// 销毁实体
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer);
	/// 时间函数
	virtual	void onTimer(unsigned int nTickOffset);

public:
	/// 处理客户端消息
	void		onClientMessage(CExchangeHead& rExchangeHead, unsigned int nMessageID, Message* pMessage);
	/// 处理数据库消息
	void		OnDBMessage(CDBResponse* pMsg);
	/// 处理等待玩家进入游戏
	void		processWaitEnterGame(CLoginPlayer* pLoginPlayer, Message* pMessage);
	///  一个Socket断开
	void		onSocketDisconnect(int nSocketIndex);
	/// 加载数据库里分配角色ID的字段
	void		loadAllocateRoleId();
	/// 加载数据库里分配角色ID的处理
	void		onLoadAllocateRoleId(CDBResponse& rResponse);

	unsigned int getBeginRoleID(int nServerId)
	{
		if (nServerId <= 0 || nServerId > MAX_ROLE_PER_SERVER)
		{
			return 0;
		}
		return (nServerId - 1) * MAX_SERVER_ID + 1;
	}

	unsigned int allocateRoleID(int nServerId);

public:
	unsigned int getAllocateRoleId(int nIndex)
	{
		if (nIndex < 0 || nIndex >= MAX_SERVER_ID)
		{
			return 0;
		}

		return mAllocateRoleId[nIndex];
	}

	void		setAllocateRoleId(int nIndex, int nRoleID)
	{
		if (nIndex < 0 || nIndex >= MAX_SERVER_ID)
		{
			return;
		}
		mAllocateRoleId[nIndex] = nRoleID;
	}
private:
	LOGIN_LIST			mLoginList;						// 登录列表
	CAutoResetTimer		mLoginCheckTime;				// 登录校验时间
	unsigned int		mAllocateRoleId[MAX_SERVER_ID];	// 角色ID分配器
	bool				mCheckResult;
};
#endif