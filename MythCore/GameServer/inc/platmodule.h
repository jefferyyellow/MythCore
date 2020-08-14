#ifndef __PLATMODULE_H__
#define __PLATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "commontype.h"
#include "platconfig.h"
using namespace Myth;
class CDBResponse;
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
	void processRecharge(const char* pOrderID, const char* pGoodsID, uint nRoleID,
		uint nAccountID, short nChannelID, short nServerID, double dRechargeMoney);

	/// �����ֵ������DB�ص�
	void onInsertRechargeCache(CDBResponse& rResponse);
	/// ���س�ֵ����
	void loadRechargeCache(CEntityPlayer* pPlayer);
	/// ���س�ֵ�����DB�ص�
	void onLoadRechargeCache(CDBResponse& rResponse);
	/// ��ֵ�ɹ���DB�ص�
	void onRechargeSuccess(CDBResponse& rResponse);


public:
	CRechargeConfig&		getRechargeConfig(){return mRechargeConfig;}

private:
	CRechargeConfig			mRechargeConfig;
};
#endif