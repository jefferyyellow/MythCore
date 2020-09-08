#ifndef __PROPERTYMODULE_H__
#define __PROPERTYMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "gmcommandmanager.h"
#include "messagefactory.h"
#include "timeutility.h"
#include "logintype.h"
using namespace Myth;
class CEntityPlayer;
class CEntity;
class CDBResponse;
class CEntityCreator;
class CPropertyModule : public CLogicModule, public CSingleton < CPropertyModule >
{
	friend class CSingleton < CPropertyModule >;
private:
	CPropertyModule();
	virtual ~CPropertyModule()
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
	virtual		void onTimer(unsigned int nTickOffset);
	/// 加载配置文件
	virtual		void onLoadConfig();

public:
	void		onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);

public:
	void			checkSavePlayerTimer();
public:
	/// 加载玩家信息
	void			onLoadPlayerInfo(CDBResponse& rResponse);
	/// 加载玩家基础属性
	void			onLoadPlayerBaseProperty(CDBResponse& rResponse);
	/// 玩家属性加载完成
	void			onLoadComplete(CEntityPlayer* pPlayer);
public:
	/// 玩家存盘
	void			savePlayer(CEntityPlayer& rPlayer);
	/// 保存玩家信息
	void			savePlayerInfo(CEntityPlayer& rPlayer);
	/// 保存玩家基本属性
	void			savePlayerBaseProperty(CEntityPlayer& rPlayer);
	/// 玩家存盘完成
	void			onSavePlayerComplete(CEntityPlayer& rPlayer);

public:
	/// 玩家离开游戏
	void			playerLeaveGame(CEntityPlayer& rPlayer, EmLeaveReason eReason);
	/// 删除玩家实例
	void			destroyPlayer(CEntityPlayer& rPlayer);
	/// 新玩家处理
	void			setNewPlayerValue(CEntityPlayer* pPlayer);
	/// 创建实体后的处理
	void			onCreateEntity(CEntityCreator* pCreator, CEntity* pEntity);
private:
	/// GM命令请求
	void		onGMCommandRequest(CEntityPlayer& rPlayer, Message* pMessage);
	/// 玩家离开游戏的请求
	void		onLeaveGameRequest(CEntityPlayer& rPlayer, Message* pMessage);
	/// 发送玩家基本信息通知
	void		sendPlayerBaseInfoNotify(CEntityPlayer* pPlayer);
	/// 发送玩家道具信息通知
	void		sendPlayerItemInfoNotify(CEntityPlayer* pPlayer);
	/// 发送玩家任务信息通知
	void		sendPlayerTaskInfoNotify(CEntityPlayer* pPlayer);
	/// 玩家技能信息通知
	void		sendPlayerSkillInfoNotify(CEntityPlayer* pPlayer);
	//// 得到玩家属性请求
	void		onGetPlayerPropertyRequest(CEntityPlayer* pPlayer, Message* pMessage);

public:
	CGMCommandManager&		getGMCmdManager(){ return mGMCmdManager; }
	/// 踢出所以玩家
	void		kickAllPlayer();

private:
	CGMCommandManager		mGMCmdManager;
	CAutoResetTimer			mSavePlayerTimer;
};
#endif