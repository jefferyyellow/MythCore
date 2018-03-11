#ifndef __ROOMMODULE_H__
#define __ROOMMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
using namespace Myth;
class CRoomModule : public CLogicModule, public CSingleton<CRoomModule>
{
	friend class CSingleton<CRoomModule>;
private:
	CRoomModule()
	{

	}

	~CRoomModule()
	{

	}

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
	void onCreateRoom(CEntityPlayer* pPlayer, Message* pMessage);
	void onLeaveRoom(CEntityPlayer* pPlayer, Message* pMessage);
	void onEnterGame(CEntityPlayer* pPlayer, Message* pMessage);
	void onReady(CEntityPlayer* pPlayer, Message* pMessage);
	void onCheckPoker(CEntityPlayer* pPlayer, Message* pMessage);
};
#endif