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
void CPlatModule::onCreatePlayer(CEntityPlayer& rPlayer)
{
	loadRechargeCache(rPlayer);
}

/// 销毁实体
void CPlatModule::onDestroyPlayer(CEntityPlayer& rPlayer)
{

}

/// 时间函数
void CPlatModule::onTimer(unsigned int nTickOffset)
{

}

/// 加载配置文件
void CPlatModule::onLoadConfig()
{
	mRechargeConfig.LoadRechargeConfig("gameserverconfig/recharge/recharge.xml");
}

/// 处理充值
void CPlatModule::processRecharge(const char* pOrderID, const char* pGoodsID, uint nRoleID,
	uint nAccountID, short nChannelID, short nServerID, double dRechargeMoney)
{
	if (NULL == pGoodsID || NULL == pOrderID)
	{
		return;
	}

	uint nOrderCRC = crc32((unsigned char*)pOrderID, strlen(pOrderID));
	// 插入充值缓存表中
	CDBModule::Inst()->pushDBTask(nRoleID, emSessionType_InsertRechargeCache, nRoleID, 0, "call InsertRechargeCache(%u, '%s', '%s',%u, %u, %u, %u, %f, %d)",
		nOrderCRC, pOrderID, pGoodsID, nRoleID, nAccountID, nChannelID, nServerID, dRechargeMoney, CTimeManager::Inst()->getCurrTime());

}

/// 插入充值缓存中DB回调
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
	if (NULL != pEntityPlayer)
	{
		loadRechargeCache(*pEntityPlayer);
	}
}


/// 加载充值缓存
void CPlatModule::loadRechargeCache(CEntityPlayer& rPlayer)
{
	// 玩家离线或者不是游戏状态
	if (emPlayerStatus_Gameing != rPlayer.getPlayerStauts())
	{
		return;
	}

	uint nRoleID = rPlayer.getRoleID();
	CDBModule::Inst()->pushDBTask(nRoleID, emSessionType_LoadRechargeCache, nRoleID, 0, "call LoadRechargeCache(%u)", nRoleID);
}

/// 加载充值缓存的DB回调
void CPlatModule::onLoadRechargeCache(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}

	uint nRoleID = rResponse.mParam1;
	CEntityPlayer* pEntityPlayer = CSceneJob::Inst()->getPlayerByRoleID(nRoleID);
	// 玩家离线或者不是游戏状态
	if (NULL == pEntityPlayer || emPlayerStatus_Gameing != pEntityPlayer->getPlayerStauts())
	{
		return;
	}

	// 表示处理完成
	if (rResponse.mRowNum <= 0)
	{
		return;
	}
	uint nID = 0;
	char strOrderID[STR_LENGTH_64] = {0};
	char strGoodsID[STR_LENGTH_64] = {0};
	int nRechargeMoney = 0;
	uint nIDCRC = 0;

	int nResult = SUCCESS;
	for (int i = 0; i < rResponse.mRowNum; ++ i)
	{
		nID = rResponse.getUInt();
		rResponse.getString(strOrderID, sizeof(strOrderID));
		rResponse.getString(strGoodsID, sizeof(strGoodsID));
		nRechargeMoney = rResponse.getUInt();
		nIDCRC = crc32((unsigned char*)strGoodsID, strlen(strGoodsID));
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

/// 充值成功的DB回调
void CPlatModule::onRechargeSuccess(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}

}