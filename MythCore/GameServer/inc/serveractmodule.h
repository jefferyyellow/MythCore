#ifndef __SERVERACTMODULE_H__
#define __SERVERACTMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "serveractivitytype.h"

using namespace Myth;

class CEntityPlayer;
class CServerActivity;

/// 开服活动模块
class CServerActModule : public CLogicModule, public CSingleton <CServerActModule>
{
	friend class CSingleton <CServerActModule> ;
private:
	CServerActModule();
	~CServerActModule();

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
	virtual void onCreatePlayer(CEntity* pEntity);
	/// 销毁实体
	virtual void onDestroyPlayer(CEntity* pEntity);
	/// 时间函数
	virtual	void onTimer(unsigned int nTickOffset);

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
	time_t				timeToMorning(time_t tTime);

public:
	void				loadServerActivityConfig(const char* pConfigFile);
	CServerActivity*	createServerActivity(EmServerActType emServerActType);

private:
	/// 服务器开服时间
	time_t				mServerStartTime;
};
#endif