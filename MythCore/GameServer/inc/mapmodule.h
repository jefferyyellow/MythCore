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
	/// ����������
	virtual void onLaunchServer();
	/// ������ɼ��
	virtual bool onCheckLaunch();
	/// �����������ɹ�
	virtual void onLaunchComplete();
	/// �˳�������
	virtual void onExitServer();
	/// ��һ�쵽��
	virtual void onNewDayCome();
	/// ��һ�ܵ���
	virtual void onNewWeekCome();
	/// ����ʵ��
	virtual void onCreatePlayer(CEntityPlayer* pPlayer);
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer);
	/// ʱ�亯��
	virtual	void onTimer(unsigned int nTickOffset);
	/// ���������ļ�
	virtual	void onLoadConfig();

public:
	void		onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
public:
	int			teleportEntity(CEntity* pEntity, unsigned short nMapID, CMythPoint& rPos);
	/// ����ʵ��
	CEntity*	createEntity(CEntityCreator* pCreator);
	/// ɾ��ʵ��
	void		destroyEntity(CEntity* pEntity);
	/// ���ݵ�ͼ��ID����ͼID����ͼ����������ͼ
	CMap*		createMapFromConfig(unsigned short nLineID, unsigned short nMapID, int nMapIndex);
	/// ʵ���ƶ�
	void		onEntityMove(CEntity* pEntity, CMythPoint& rDesPos);

private:
	/// �㲥�������Ŀɼ����
	void		broadCastVisiblePlayer(CEntity* pEntity, unsigned short nMessageID, Message* pMessage);
	/// �㲥��ͼ���������
	void		broadCastMapPlayer(CEntity* pEntity, unsigned short nMessageID, Message* pMessage);
	/// ��������ƶ�������
	void		onMessagePlayerMoveRequest(CEntityPlayer* pPlayer, Message* pMessage);
	/// ���ʹ�������ƶ��Ļ�Ӧ
	void		sendPlayerMoveResponse(CEntityPlayer* pPlayer, int nResult);
	/// ������Ҵ��͵���Ϣ
	void		onMessagePlayerTeleportRequest(CEntityPlayer* pPlayer, Message* pMessage);
	// ������Ҵ��ͻ�Ӧ
	void		sendPlayerTeleportResponse(CEntityPlayer* pPlayer, int nResult);

private:
	/// ��ʱ���ټ�ʱ��
	CAutoResetTimer		mDestroyTimer;
};
#endif