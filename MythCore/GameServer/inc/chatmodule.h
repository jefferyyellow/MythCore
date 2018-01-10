#ifndef __CHATMODULE_H__
#define __CHATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
using namespace Myth;
class CChatModule : public CLogicModule, public CSingleton<CChatModule>
{
	friend class CSingleton<CChatModule>;
private:
	CChatModule();
	~CChatModule();

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
	virtual void onCreatePlayer(CEntity* pEntity);
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntity* pEntity);
	/// ʱ�亯��
	virtual	void onTimer(unsigned int nTickOffset);

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

};
#endif