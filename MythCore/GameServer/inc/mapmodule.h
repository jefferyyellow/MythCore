#ifndef __MAPMODULE_H__
#define __MAPMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
#include "logicmodule.h"

class CEntity;
class CEntityPlayer;
class CEntityCharacter;
class CMythPoint;
using namespace Myth;
class CMapModule : public CLogicModule, public CSingleton < CMapModule >
{
	friend class CSingleton < CMapModule > ;
private:
	CMapModule()
	{
	}
	~CMapModule()
	{
	}

public:
	virtual		void	OnTimer(unsigned int nTickOffset);

public:
	void		onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
public:
	int			teleportEntity(CEntity* pEntity, unsigned short nMapID, CMythPoint& rPos);

public:
	/// 广播给附近的可见玩家
	void		broadCastVisiblePlayer(CEntity* pEntity, unsigned short nMessageID, Message* pMessage);
	/// 实体移动
	void		onEntityMove(CEntityCharacter* pEntity, CMythPoint& rDesPos);
	/// 处理玩家移动的请求
	void		onMessagePlayerMoveRequest(CEntityPlayer* pPlayer, Message* pMessage);
	/// 发送处理玩家移动的回应
	void		sendPlayerMoveResponse(CEntityPlayer* pPlayer, int nResult);
	/// 处理玩家传送的消息
	void		onMessagePlayerTeleportRequest(CEntityPlayer* pPlayer, Message* pMessage);
	// 发送玩家传送回应
	void		sendPlayerTeleportResponse(CEntityPlayer* pPlayer, int nResult);
};
#endif