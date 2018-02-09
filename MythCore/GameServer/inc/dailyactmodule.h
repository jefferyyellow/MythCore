#ifndef __DAILYACTMODULE_H__
#define __DAILYACTMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
using namespace Myth;
class CEntityPlayer;
// �ճ��ģ��
class CDailyActModule : public CLogicModule, public CSingleton <CDailyActModule>
{
	friend class CSingleton <CDailyActModule> ;
private:
	CDailyActModule();
	~CDailyActModule();

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