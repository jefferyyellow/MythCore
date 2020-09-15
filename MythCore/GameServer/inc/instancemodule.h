#ifndef __INSTANCEMODULE_H__
#define __INSTANCEMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "messagefactory.h"
#include "instancetype.h"
#include "timeutility.h"
using namespace Myth;
class CInstance;
class CInstanceConfig;
class CEntityOgre;
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
	// 加载所有的副本配置
	void loadAllInstConfig(const char* pFindPath, const char* pPath);
	void onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);

public:
	// 检查玩家是否能进副本
	int checkPlayerEnterInstance(CEntityPlayer& rPlayer, int nInstanceType, int nInstanceID);
	// 玩家进入副本的请求
	void onEnterInstanceRequest(CEntityPlayer& rPlayer, Message* pMessage);
	// 玩家进入副本的回应
	void sendEnterInstanceResponse(CEntityPlayer& rPlayer, int nResult, int nInstanceID);
	// 玩家离开副本的请求
	void onLeaveInstanceRequest(CEntityPlayer& rPlayer, Message* pMessage);
	// 玩家离开副本的回应
	void sendLeaveInstanceResponse(CEntityPlayer& rPlayer, int nResult);

public:
	/// 创建副本
	CInstance*	createInstance(int nInstanceID);
	/// 销毁副本
	void		destoryInstance(CInstance* pInstance);
	/// 检查所有的副本时间
	void		checkAllInstanceTime();
	/// ----- 副本事件相关 -----
	/// 怪物死亡
	void		onOgreDead(CEntityOgre *pOgre);
	/// 玩家死亡
	void		onPlayerDead(CEntityPlayer& rPlayer);
	/// 玩家离开
	void		onPlayerLeave(CEntityPlayer& rPlayer);
	/// ----- 副本事件相关 -----

public:
	CInstanceConfig*	mAllInstConfig[MAX_INSTANCE_ID];
	time_t				mLastCheckTime;
};
#endif