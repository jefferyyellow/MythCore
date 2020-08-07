#ifndef __PLATMODULE_H__
#define __PLATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "commontype.h"
using namespace Myth;
class CPlatModule : public CLogicModule, public CSingleton<CPlatModule>
{
	friend class CSingleton<CPlatModule>;
private:
	CPlatModule();
	~CPlatModule();

	void init();

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
	/// �����ֵ
	void processRecharge(uint nAccountID, short nChannelID, short nServerID, uint nRoleID,
		char* pGoodsID, char* pOrderID, double dRechargeMoney);
	/// ����������ҵĳ�ֵ
	void processRechargeOffline(uint nAccountID, short nChannelID, short nServerID, uint nRoleID,
		char* pGoodsID, char* pOrderID, double dRechargeMoney);

};
#endif