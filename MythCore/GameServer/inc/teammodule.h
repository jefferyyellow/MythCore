#ifndef __TEAMMODULE_H__
#define __TEAMMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
using namespace Myth;
class CTeamModule : public CLogicModule, public CSingleton<CTeamModule>
{
	friend class CSingleton<CTeamModule>;

private:
	CTeamModule();
	~CTeamModule();

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
	virtual void onCreatePlayer(CEntityPlayer& rPlayer);
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntityPlayer& rPlayer);
	/// ʱ�亯��
	virtual	void onTimer(unsigned int nTickOffset);
	/// ���������ļ�
	virtual	void onLoadConfig();

public:
	void onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);
};
#endif