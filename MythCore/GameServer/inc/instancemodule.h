#ifndef __INSTANCEMODULE_H__
#define __INSTANCEMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "messagefactory.h"
using namespace Myth;
class CInstance;
class CInstanceModule : public CLogicModule, public CSingleton < CInstanceModule >
{
	friend class CSingleton<CInstanceModule>;
private:
	CInstanceModule();
	~CInstanceModule();

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
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

public:
	CInstance* createInstance(int nInstance);
};
#endif