#ifndef __DAILYACTMODULE_H__
#define __DAILYACTMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
using namespace Myth;
class CEntityPlayer;
// 日常活动模块
class CDailyActModule : public CLogicModule, public CSingleton <CDailyActModule>
{
	friend class CSingleton <CDailyActModule> ;
private:
	CDailyActModule();
	~CDailyActModule();

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

};
#endif