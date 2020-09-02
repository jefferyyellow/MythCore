#ifndef __INSTANCEMODULE_H__
#define __INSTANCEMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "messagefactory.h"
using namespace Myth;
class CInstance;
class CInstanceModule : public CLogicModule, public CSingleton < CInstanceModule >
{
	friend class CSingleton<CInstanceModule>;
private:
	CInstanceModule();
	~CInstanceModule();

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
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

public:
	CInstance* createInstance(int nInstance);
};
#endif