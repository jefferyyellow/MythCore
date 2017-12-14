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
	/// �㲥�������Ŀɼ����
	void		broadCastVisiblePlayer(CEntity* pEntity, unsigned short nMessageID, Message* pMessage);
	/// ʵ���ƶ�
	void		onEntityMove(CEntityCharacter* pEntity, CMythPoint& rDesPos);
	/// ��������ƶ�������
	void		onMessagePlayerMoveRequest(CEntityPlayer* pPlayer, Message* pMessage);
	/// ���ʹ�������ƶ��Ļ�Ӧ
	void		sendPlayerMoveResponse(CEntityPlayer* pPlayer, int nResult);
	/// ������Ҵ��͵���Ϣ
	void		onMessagePlayerTeleportRequest(CEntityPlayer* pPlayer, Message* pMessage);
	// ������Ҵ��ͻ�Ӧ
	void		sendPlayerTeleportResponse(CEntityPlayer* pPlayer, int nResult);
};
#endif