#ifndef __SERVERACTMODULE_H__
#define __SERVERACTMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "serveractivitytype.h"
#include "servercommon.h"

using namespace Myth;

class CEntityPlayer;
class CServerActivity;

/// 玩家的开服活动的模块数据每天一刷新，如果有重启服务器，就会额外刷新
/// 所以会记录服务器启动时间
/// 开服活动模块
class CServerActModule : public CLogicModule, public CSingleton <CServerActModule>
{
	friend class CSingleton <CServerActModule> ;
private:
	CServerActModule();
	~CServerActModule();
	void init()
	{
        mServerOpenTime = 0;
        mServerStartTime = 0;
	}

public:
	typedef	vector<CServerActivity*>	SERVER_ACT_LIST;
	typedef vector<string>				SERVER_LOAD_LIST;
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
	void				onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
	void				onGetServerActRequest(CEntityPlayer* pPlayer, Message* pMessage);
	void				sendGetServerActResponse(CEntityPlayer* pPlayer);

public:
	/// 加载开放活动配置
	void				loadServerActivityConfig(const char* pConfigFile);
	/// 加载详细活动配置
	void				loadSpecifyActivityConfig(const char* pConfigFile);

	CServerActivity*	createServerActivity(EmServerActType emServerActType);
	/// 清算结束的活动
	void				clearEndedActivity();
	/// 每日刷新
	void				dailyRefresh(CEntityPlayer* pPlayer);
	CServerActivity*	getServerActivity(int nActUniqueID);
	CServerActivity*	getServerActivity(int nType, int nSubType, int nID);
	/// autocode
    time_t getServerOpenTime(){ return mServerOpenTime;}
    void setServerOpenTime(time_t value){ mServerOpenTime = value;}

    time_t getServerStartTime(){ return mServerStartTime;}
    void setServerStartTime(time_t value){ mServerStartTime = value;}

    SERVER_ACT_LIST& getServerActList(){ return mServerActList;}
	/// end autocode

private:
	/// 服务器开服时间
	time_t					mServerOpenTime;
	/// 服务器启动时间
	time_t					mServerStartTime;
	/// 开服活动列表
	SERVER_ACT_LIST			mServerActList;
};
#endif