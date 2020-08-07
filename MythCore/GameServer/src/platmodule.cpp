#include "platmodule.h"
#include "scenejob.h"
#include "entityplayer.h"
CPlatModule::CPlatModule()
{

}

CPlatModule::~CPlatModule()
{

}

void CPlatModule::init()
{

}

/// ����������
void CPlatModule::onLaunchServer()
{

}

/// ������ɼ��
bool CPlatModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CPlatModule::onLaunchComplete()
{

}

/// �˳�������
void CPlatModule::onExitServer()
{

}

/// ��һ�쵽��
void CPlatModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CPlatModule::onNewWeekCome()
{

}

/// ����ʵ��
void CPlatModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
void CPlatModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

/// ʱ�亯��
void CPlatModule::onTimer(unsigned int nTickOffset)
{

}

/// �����ֵ
void CPlatModule::processRecharge(uint nAccountID, short nChannelID, short nServerID, uint nRoleID,
	char* pGoodsID, char* pOrderID, double dRechargeMoney)
{
	if (NULL == pGoodsID || NULL == pOrderID)
	{
		return;
	}

	CEntityPlayer* pEntityPlayer = CSceneJob::Inst()->getPlayerByRoleID(nRoleID);
	// ������߻��߲�����Ϸ״̬
	if (NULL == pEntityPlayer || emPlayerStatus_Gameing != pEntityPlayer->getPlayerStauts())
	{
		processRechargeOffline(nAccountID, nChannelID, nServerID, nRoleID, pGoodsID, pOrderID, dRechargeMoney);
		return;
	}

	pEntityPlayer->getVIPUnit().processRecharge(pGoodsID, pOrderID, dRechargeMoney);
}

/// ����������ҵĳ�ֵ
void CPlatModule::processRechargeOffline(uint nAccountID, short nChannelID, short nServerID, uint nRoleID,
	char* pGoodsID, char* pOrderID, double dRechargeMoney)
{

}