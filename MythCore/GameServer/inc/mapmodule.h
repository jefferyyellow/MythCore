#ifndef __MAPMODULE_H__
#define __MAPMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
#include "logicmodule.h"
#include "timeutility.h"

class CEntity;
class CEntityPlayer;
class CEntityCharacter;
class CMythPoint;
class CEntityNPC;
class CEntityCreator;
class CMap;
using namespace Myth;
class CMapModule : public CLogicModule, public CSingleton < CMapModule >
{
	friend class CSingleton < CMapModule > ;
private:
	CMapModule();
	~CMapModule()
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
	/// 加载配置文件
	virtual	void onLoadConfig();

public:
	void		onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
public:
	int			teleportEntity(CEntity* pEntity, unsigned short nMapID, CMythPoint& rPos);
	/// 创建实体
	CEntity*	createEntity(CEntityCreator* pCreator);
	/// 删除实体
	void		destroyEntity(CEntity* pEntity);
	/// 根据地图线ID，地图ID，地图索引创建地图
	CMap*		createMapFromConfig(unsigned short nLineID, unsigned short nMapID, int nMapIndex);
	/// 实体移动
	void		onEntityMove(CEntity* pEntity, CMythPoint& rDesPos);

private:
	/// 广播给附近的可见玩家
	void		broadCastVisiblePlayer(CEntity* pEntity, unsigned short nMessageID, Message* pMessage);
	/// 广播地图上所有玩家
	void		broadCastMapPlayer(CEntity* pEntity, unsigned short nMessageID, Message* pMessage);
	/// 处理玩家移动的请求
	void		onMessagePlayerMoveRequest(CEntityPlayer* pPlayer, Message* pMessage);
	/// 发送处理玩家移动的回应
	void		sendPlayerMoveResponse(CEntityPlayer* pPlayer, int nResult);
	/// 处理玩家传送的消息
	void		onMessagePlayerTeleportRequest(CEntityPlayer* pPlayer, Message* pMessage);
	// 发送玩家传送回应
	void		sendPlayerTeleportResponse(CEntityPlayer* pPlayer, int nResult);

private:
	/// 定时销毁计时器
	CAutoResetTimer		mDestroyTimer;
};
#endif