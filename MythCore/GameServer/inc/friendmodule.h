#ifndef __FRIENDMODULE_H__
#define __FRIENDMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
using namespace Myth;
class CFriendModule : public CLogicModule, public CSingleton<CFriendModule>
{
	friend class CSingleton<CFriendModule>;
private:
	CFriendModule();
	~CFriendModule();

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
	virtual void onCreatePlayer(CEntityPlayer& rPlayer);
	/// 销毁实体
	virtual void onDestroyPlayer(CEntityPlayer& rPlayer);
	/// 时间函数
	virtual	void onTimer(unsigned int nTickOffset);
	/// 加载配置文件
	virtual	void onLoadConfig();

};
#endif