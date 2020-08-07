#ifndef __PLATMODULE_H__
#define __PLATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "commontype.h"
using namespace Myth;
class CPlatModule : public CLogicModule, public CSingleton<CPlatModule>
{
	friend class CSingleton<CPlatModule>;
private:
	CPlatModule();
	~CPlatModule();

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

public:
	/// 处理充值
	void processRecharge(uint nAccountID, short nChannelID, short nServerID, uint nRoleID,
		char* pGoodsID, char* pOrderID, double dRechargeMoney);
	/// 处理离线玩家的充值
	void processRechargeOffline(uint nAccountID, short nChannelID, short nServerID, uint nRoleID,
		char* pGoodsID, char* pOrderID, double dRechargeMoney);

};
#endif