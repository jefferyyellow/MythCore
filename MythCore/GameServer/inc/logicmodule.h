#ifndef __LOGICMODULE_H__
#define __LOGICMODULE_H__
/// 逻辑模块
class CEntityPlayer;
class CLogicModule
{
public:
	CLogicModule()
	{}

	virtual ~CLogicModule()
	{}
public:
	/// 启动服务器
	virtual void onLaunchServer() = 0;
	/// 启动完成检查
	virtual bool onCheckLaunch() = 0;
	/// 服务器启动成功
	virtual void onLaunchComplete() = 0;
	/// 退出服务器
	virtual void onExitServer() = 0;
	/// 新一天到来
	virtual void onNewDayCome() = 0;
	/// 新一周到来
	virtual void onNewWeekCome() = 0;
	/// 建立实体
	virtual void onCreatePlayer(CEntityPlayer* pPlayer) = 0;
	/// 销毁实体
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer) = 0;
	/// 时间函数
	virtual	void onTimer(unsigned int nTickOffset) = 0;
};
#endif // !__LOGICMODULE_H__
