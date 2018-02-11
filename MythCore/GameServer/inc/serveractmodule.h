#ifndef __SERVERACTMODULE_H__
#define __SERVERACTMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "serveractivitytype.h"

using namespace Myth;

class CEntityPlayer;
class CServerActivity;

/// �����ģ��
class CServerActModule : public CLogicModule, public CSingleton <CServerActModule>
{
	friend class CSingleton <CServerActModule> ;
private:
	CServerActModule();
	~CServerActModule();

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
	time_t				timeToMorning(time_t tTime);

public:
	void				loadServerActivityConfig(const char* pConfigFile);
	CServerActivity*	createServerActivity(EmServerActType emServerActType);

private:
	/// ����������ʱ��
	time_t				mServerStartTime;
};
#endif