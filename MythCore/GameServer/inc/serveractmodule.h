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

	void init();

	// 清空开服活动
	void clearServerActivity();

public:
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
	/// 清算结束的活动
	void				clearEndedActivity();
	/// 得到对应的开服活动
	CServerActivity*	getServerActivity(int nActivityID);
	/// 刷新活动进度
	void				refreshProcess(EmSvrActType eType, CEntityPlayer& rPlayer, int nParam1, int nParam2);
	/// 初始化类型函数
	void				initActivityTypeFunc(int nType, int nFuncType, const char* pFuncName);
public:
	/// autocode
    time_t getServerOpenTime(){ return mServerOpenTime;}
    void setServerOpenTime(time_t value){ mServerOpenTime = value;}

    time_t getServerStartTime(){ return mServerStartTime;}
    void setServerStartTime(time_t value){ mServerStartTime = value;}
	/// end autocode

private:
	/// 服务器开服时间
	time_t					mServerOpenTime;
	/// 服务器启动时间
	time_t					mServerStartTime;
	/// 开服活动
	CServerActivity*		mServerActivity[MAX_SERVER_ACT_NUM];
	/// 开服活动
	CServerActivity*		mAvailActivity[MAX_SERVER_ACT_NUM];
	/// 有效的开服活动数目
	int						mAvailActivityNum;
};
#endif