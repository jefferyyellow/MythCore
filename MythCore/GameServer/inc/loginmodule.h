#ifndef __LOGINMODULE_H__
#define __LOGINMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
#include "servercommon.h"
#include "logintype.h"
#include "logicmodule.h"
#include "timeutility.h"
#include <map>

using namespace Myth;
class CDBResponse;
class CLoginPlayer;
#define  MAKE_LOGIN_KEY(AccountID, ChannelID, ServerID) ((ChannelID << 48) | (ServerID << 32) | AccountID)
class CLoginModule : public CLogicModule, public CSingleton < CLoginModule >
{
	friend class CSingleton < CLoginModule > ;
public:
	typedef std::map<short, int> LOGIN_LIST;
	typedef std::map<uint64, int> VERIFY_LIST;

private:
	CLoginModule();
	~CLoginModule();
	void init();

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
	/// 加载配置文件
	virtual	void onLoadConfig();

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
	/// 增加正在校验的玩家
	bool		addVerifyPlayer(short nChannelId, short nServerId, int nAccountId, int nLoginObjId);
	/// 是否是正在校验的玩家
	bool		checkVerifyPlayer(short nChannelId, short nServerId, int nAccountId);
	/// 删除正在校验的玩家
	void		removeVerifyPlayer(short nChannelId, short nServerId, int nAccountId);
	/// 得到登录玩家数据
	CLoginPlayer* getLoginPlayer(int nSocketIndex);

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
	VERIFY_LIST			mVerifyList;					// 校验列表
	CAutoResetTimer		mLoginCheckTime;				// 登录校验时间
	unsigned int		mAllocateRoleId[MAX_SERVER_ID];	// 角色ID分配器
	bool				mCheckResult;
};
#endif