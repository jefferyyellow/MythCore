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

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
	void onCreateRoom(CEntityPlayer* pPlayer, Message* pMessage);
	void onLeaveRoom(CEntityPlayer* pPlayer, Message* pMessage);
	void onEnterGame(CEntityPlayer* pPlayer, Message* pMessage);
	void onReady(CEntityPlayer* pPlayer, Message* pMessage);
	void onCheckPoker(CEntityPlayer* pPlayer, Message* pMessage);
};
#endif