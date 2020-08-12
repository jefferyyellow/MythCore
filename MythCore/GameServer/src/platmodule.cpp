#include "platmodule.h"
#include "scenejob.h"
#include "entityplayer.h"
#include "crc32.h"
#include "dbmodule.h"
#include "timemanager.h"
#include "errcode.h"
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
	mRechargeConfig.LoadRechargeConfig("gameserverconfig/recharge/recharge.xml");
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
void CPlatModule::processRecharge(char* pOrderID, char* pGoodsID, uint nRoleID,
	uint nAccountID, short nChannelID, short nServerID, double dRechargeMoney)
{
	if (NULL == pGoodsID || NULL == pOrderID)
	{
		return;
	}

	uint nOrderCRC = crc32((unsigned char*)pOrderID, strlen(pOrderID));
	insertRechargeCache(nOrderCRC, pOrderID, pGoodsID, nRoleID, nAccountID, nChannelID, nServerID, dRechargeMoney);
}


/// �����ֵ������
void CPlatModule::insertRechargeCache(uint nOrderCRC, char* pOrderID, char* pGoodsID, uint nRoleID,
	uint nAccountID, short nChannelID, short nServerID, double dRechargeMoney)
{
	CDBModule::Inst()->pushDBTask(nRoleID, emSessionType_InsertRechargeCache, nRoleID, 0, "call InsertRechargeCache(%u, '%s', '%s',%u, %u, %u, %u, %f, %d)",
		nOrderCRC, pOrderID, pGoodsID, nRoleID, nAccountID, nChannelID, nServerID, dRechargeMoney, CTimeManager::Inst()->getCurrTime());

}

/// �����ֵ������DB�ص�
void CPlatModule::onInsertRechargeCache(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}

	int nResult = rResponse.getInt();
	if (nResult != 0)
	{
		return;
	}

	CEntityPlayer* pEntityPlayer = CSceneJob::Inst()->getPlayerByRoleID(rResponse.mParam1);
	// ������߻��߲�����Ϸ״̬
	if (NULL == pEntityPlayer || emPlayerStatus_Gameing != pEntityPlayer->getPlayerStauts())
	{
		return;
	}
	CDBModule::Inst()->pushDBTask(pEntityPlayer->getRoleID(), emSessionType_LoadRechargeCache, rResponse.mParam1, 0, "call LoadRechargeCache(%u)", pEntityPlayer->getRoleID());
}

/// ���س�ֵ�����DB�ص�
void CPlatModule::onLoadRechargeCache(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}

	uint nRoleID = rResponse.mParam1;
	CEntityPlayer* pEntityPlayer = CSceneJob::Inst()->getPlayerByRoleID(nRoleID);
	// ������߻��߲�����Ϸ״̬
	if (NULL == pEntityPlayer || emPlayerStatus_Gameing != pEntityPlayer->getPlayerStauts())
	{
		return;
	}

	// ��ʾ�������
	if (rResponse.mRowNum <= 0)
	{
		return;
	}
	uint nID = 0;
	uint nIDCRC = 0;

	char strOrderID[STR_LENGTH_64] = {0};
	char strGoodsID[STR_LENGTH_64] = {0};
	int nRechargeMoney = 0;

	int nResult = SUCCESS;
	for (int i = 0; i < rResponse.mRowNum; ++ i)
	{
		nID = rResponse.getUInt();
		nIDCRC = rResponse.getUInt();
		rResponse.getString(strOrderID, sizeof(strOrderID));
		rResponse.getString(strGoodsID, sizeof(strGoodsID));
		nRechargeMoney = rResponse.getUInt();

		CRechargeGoods* pGoods = mRechargeConfig.getGoods(nIDCRC, strGoodsID);
		if (NULL == pGoods)
		{
			LOG_ERROR("Recharge Goods is not exist, RoleID: %u, GoodsID: %s", nRoleID, strGoodsID);
			continue;
		}
		nResult = pEntityPlayer->getVIPUnit().processRecharge(pGoods, nRechargeMoney);
		if (SUCCESS == nResult)
		{
			CDBModule::Inst()->pushDBTask(pEntityPlayer->getRoleID(), emSessionType_RechargeSuccess, nRoleID, 0, "call RechargeSuccess(%u)", nID);
		}
	}
}

/// ��ֵ�ɹ���DB�ص�
void CPlatModule::onRechargeSuccess(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}
}