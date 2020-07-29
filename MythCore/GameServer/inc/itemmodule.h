#ifndef __ITEMMODULE_H__
#define __ITEMMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "shopconfig.h"
using namespace Myth;
class CEntityPlayer;
class CItemModule : public CLogicModule, public CSingleton < CItemModule >
{
	friend class CSingleton < CItemModule >;
private:
	CItemModule();
	~CItemModule();

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
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

public:
	int	getFromDropTable(int nDropTableID, int& rDropItemNum);

public:
	SHOP_CONFIG_LIST&	getShopList(){return mShopList;}
public:
	SHOP_CONFIG_LIST	mShopList;
};
#endif