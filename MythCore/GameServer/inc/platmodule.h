#ifndef __PLATMODULE_H__
#define __PLATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "commontype.h"
#include "platconfig.h"
using namespace Myth;
class CDBResponse;
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
	void processRecharge(const char* pOrderID, const char* pGoodsID, uint nRoleID,
		uint nAccountID, short nChannelID, short nServerID, double dRechargeMoney);

	/// 插入充值缓存中DB回调
	void onInsertRechargeCache(CDBResponse& rResponse);
	/// 加载充值缓存
	void loadRechargeCache(CEntityPlayer* pPlayer);
	/// 加载充值缓存的DB回调
	void onLoadRechargeCache(CDBResponse& rResponse);
	/// 充值成功的DB回调
	void onRechargeSuccess(CDBResponse& rResponse);


public:
	CRechargeConfig&		getRechargeConfig(){return mRechargeConfig;}

private:
	CRechargeConfig			mRechargeConfig;
};
#endif