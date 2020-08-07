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

/// 启动服务器
void CPlatModule::onLaunchServer()
{

}

/// 启动完成检查
bool CPlatModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CPlatModule::onLaunchComplete()
{

}

/// 退出服务器
void CPlatModule::onExitServer()
{

}

/// 新一天到来
void CPlatModule::onNewDayCome()
{

}

/// 新一周到来
void CPlatModule::onNewWeekCome()
{

}

/// 建立实体
void CPlatModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
void CPlatModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

/// 时间函数
void CPlatModule::onTimer(unsigned int nTickOffset)
{

}

/// 处理充值
void CPlatModule::processRecharge(uint nAccountID, short nChannelID, short nServerID, uint nRoleID,
	char* pGoodsID, char* pOrderID, double dRechargeMoney)
{
	if (NULL == pGoodsID || NULL == pOrderID)
	{
		return;
	}

	CEntityPlayer* pEntityPlayer = CSceneJob::Inst()->getPlayerByRoleID(nRoleID);
	// 玩家离线或者不是游戏状态
	if (NULL == pEntityPlayer || emPlayerStatus_Gameing != pEntityPlayer->getPlayerStauts())
	{
		processRechargeOffline(nAccountID, nChannelID, nServerID, nRoleID, pGoodsID, pOrderID, dRechargeMoney);
		return;
	}

	pEntityPlayer->getVIPUnit().processRecharge(pGoodsID, pOrderID, dRechargeMoney);
}

/// 处理离线玩家的充值
void CPlatModule::processRechargeOffline(uint nAccountID, short nChannelID, short nServerID, uint nRoleID,
	char* pGoodsID, char* pOrderID, double dRechargeMoney)
{

}