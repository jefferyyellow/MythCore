#include "propertymodule.h"
#include "propertyunit.h"
#include "entityplayer.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"
#include "locallogjob.h"
#include "dbmessage.h"
#include "gameserver.h"
#include "dbmodule.h"
#include "dbmodule.hxx.pb.h"
#include "entitycreator.h"
#include "objpoolimp.h"
#include "timemanager.h"
#include "errcode.h"
CPropertyModule::CPropertyModule()
:mSavePlayerTimer(1000)
{
	
}

/// 启动服务器
void CPropertyModule::onLaunchServer()
{
}

/// 启动完成检查
bool CPropertyModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CPropertyModule::onLaunchComplete()
{

}

/// 退出服务器
void CPropertyModule::onExitServer()
{
	kickAllPlayer();
}

/// 新一天到来
void CPropertyModule::onNewDayCome()
{

}

/// 新一周到来
void CPropertyModule::onNewWeekCome()
{

}

/// 建立实体
void CPropertyModule::onCreatePlayer(CEntityPlayer* pPlayer)
{
	
}

/// 销毁实体
void CPropertyModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

/// 时间函数
void CPropertyModule::onTimer(unsigned int nTickOffset)
{
	time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
	uint64 tTickCount = CSceneJob::Inst()->getLastTimerTick();

	CEntityPlayer* tArrayPlayer[CAPACITY_PLAYER];
	int nPlayerNum = 0;
	CSceneJob::PLAYER_LIST rPlayerList = CSceneJob::Inst()->getPlayerList();
	CSceneJob::PLAYER_LIST::iterator tPlayerIt = rPlayerList.begin();
	for (; tPlayerIt != rPlayerList.end(); ++tPlayerIt)
	{
		CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(tPlayerIt->second));
		if (NULL == pPlayer)
		{
			LOG_ERROR("player charid  %d don't exist", tPlayerIt->first);
			continue;
		}
		// 游戏里面
		if (pPlayer->getPlayerStauts() != emPlayerStatus_Gameing)
		{
			continue;
		}
		
		pPlayer->getTimeUnit().getTimerList().update(tTickCount);
		tArrayPlayer[nPlayerNum] = pPlayer;
		++ nPlayerNum;
	}

	if (mSavePlayerTimer.elapse(nTickOffset))
	{
		int nSavePlayerCount = 0;
		for (int i = 0; i < nPlayerNum; ++ i)
		{
			if (tTimeNow - tArrayPlayer[i]->getTimeUnit().getLastSaveTime() < 60)
			{
				continue;
			}

			savePlayer(tArrayPlayer[i]);
			tArrayPlayer[i]->getTimeUnit().setLastSaveTime(tTimeNow);
			++nSavePlayerCount;
			if (nSavePlayerCount >= 50)
			{
				break;
			}
		}
	}
}

void CPropertyModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_GM_COMMAND:
		{
			onGMCommandRequest(pPlayer, pMessage);
			break;
		}
		case ID_C2S_REQUEST_LEAVE_GAME:
		{
			onLeaveGameRequest(pPlayer, pMessage);
			break;
		}
		case ID_C2S_REQUEST_GET_PLAYER_PROPERTY:
		{
			MYTH_ASSERT(NULL != pPlayer, return);
			pPlayer->onGetPlayerPropertyRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_HEART_BEAT:
		{
			MYTH_ASSERT(NULL != pPlayer, return);
			pPlayer->getTimeUnit().onHeartBeatRequest(pMessage);
			break;
		}
		default:
			break;
	}
}


// GM命令请求
void CPropertyModule::onGMCommandRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pPlayer && NULL != pMessage, return);

	CGMCommandRequest* pGMCommandRequest = static_cast<CGMCommandRequest*>(pMessage);
	if (NULL == pGMCommandRequest)
	{
		return;
	}

	const std::string& rCommondString = pGMCommandRequest->commandstring();
	StrTokens tTokens = strSplit(rCommondString, " ");
	MYTH_ASSERT(tTokens.size() >= 1, return);

	const char* pCommandName = tTokens[0].c_str();
	char acCommandName[STR_LENGTH_32] = {0};
	strncpy(acCommandName, pCommandName, sizeof(acCommandName) - 1);
	acCommandName[sizeof(acCommandName) - 1] = '\0';

	tTokens.erase(tTokens.begin());
	mGMCmdManager.excuteCommand(acCommandName, tTokens, pPlayer);
}

/// 玩家离开游戏的请求
void CPropertyModule::onLeaveGameRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pPlayer && NULL != pMessage, return);
	// 将玩家置为下线状态
	playerLeaveGame(pPlayer);
}

/// 踢出所以玩家
void CPropertyModule::kickAllPlayer()
{
	int nCount = 0;
	CSceneJob::PLAYER_LIST& rPlayerList = CSceneJob::Inst()->getPlayerList();
	for (CSceneJob::PLAYER_LIST::iterator it = rPlayerList.begin(); it != rPlayerList.end(); ++ it)
	{
		CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
		if (NULL == pPlayer)
		{
			continue;
		}

		if (pPlayer->getPlayerStauts() == emPlayerStatus_Exiting)
		{
			continue;
		}

		playerLeaveGame(pPlayer);
		++ nCount;
		// 每次30个
		if (nCount >= 30)
		{
			break;
		}
	}
}

/// 加载玩家信息
void CPropertyModule::onLoadPlayerInfo(CDBResponse& rResponse)
{
	if (rResponse.mSqlResult != SUCCESS)
	{
		return;
	}

	if (rResponse.mRowNum <= 0)
	{
		LOG_ERROR("load player info failure, has no result return!");
		return;
	}
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(rResponse.mParam1));
	if (NULL == pPlayer)
	{
		return;
	}

	// 已经加载了，避免加载两遍，前段时间就出现了加载两次导致玩家有部分数据有点问题的情况
	if (pPlayer->getLoadStatusBit(emLoadStatus_Info) > 0)
	{
		return;
	}
	// 	select role_name, level, exp, vip_level, vip_exp, money, diamond from PlayerRole WHERE role_id=RoleID;
	
	rResponse.getString(pPlayer->getName(), PLAYER_NAME_LENGTH - 1);
	pPlayer->getPropertyUnit().setLevel((byte)rResponse.getShort());
	pPlayer->getPropertyUnit().setExp(rResponse.getInt64());
	pPlayer->getVIPUnit().setVipLevel(rResponse.getByte());
	pPlayer->getVIPUnit().setVipExp(rResponse.getInt());


	pPlayer->getItemUnit().setMoney(rResponse.getInt());
	pPlayer->getItemUnit().setDiamond(rResponse.getInt());
	
	pPlayer->getTimeUnit().setLastOffTime(rResponse.getInt());

	pPlayer->setLoadStatusBit(emLoadStatus_Info);
	onLoadComplete(pPlayer);
}

/// 加载玩家基础属性
void CPropertyModule::onLoadPlayerBaseProperty(CDBResponse& rResponse)
{
	if (rResponse.mSqlResult != SUCCESS)
	{
		return;
	}
	if (rResponse.mRowNum <= 0)
	{
		LOG_ERROR("load player base property failure, has no result return!");
		return;
	}

	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(rResponse.mParam1));
	if (NULL == pPlayer)
	{
		return;
	}

	// 已经加载了，避免加载两遍，前段时间就出现了加载两次导致玩家有部分数据有点问题的情况
	if (pPlayer->getLoadStatusBit(emLoadStatus_BaseProperty) > 0)
	{
		return;
	}

	PBItemList tBag;
	tBag.ParseFromArray(rResponse.getValue(), rResponse.getLength());
	rResponse.next();
	pPlayer->getItemUnit().getBag().setFromPB(&tBag);

	PBItemList tEquip;
	tEquip.ParseFromArray(rResponse.getValue(), rResponse.getLength());
	rResponse.next();
	pPlayer->getItemUnit().getEquip().setFromPB(&tEquip);
	// 刷新所以装备的属性
	pPlayer->getItemUnit().getEquip().refreshProperty();

	PBTaskList tTaskList;
	tTaskList.ParseFromArray(rResponse.getValue(), rResponse.getLength());
	rResponse.next();
	pPlayer->getTaskUnit().setFromPB(&tTaskList);

	pPlayer->setLoadStatusBit(emLoadStatus_BaseProperty);
	onLoadComplete(pPlayer);
}

/// 玩家属性加载完成
void CPropertyModule::onLoadComplete(CEntityPlayer* pPlayer)
{
	// 玩家属性是否加载完成
	if (NULL == pPlayer || emLoadStatusAll != pPlayer->getLoadStatus())
	{
		return;
	}
	// 登录完成删除
	CLoginPlayer* pLoginPlayer = CLoginModule::Inst()->getLoginPlayer(pPlayer->getExchangeHead().mSocketIndex);
	if (NULL != pLoginPlayer)
	{
		pLoginPlayer->setDelState(emLoginDelState_Complete);
		pLoginPlayer->setLoginState(emLoginState_None);
	}

	CTimeUnit& rTimeUnit = pPlayer->getTimeUnit();
	pPlayer->setPlayerStauts(emPlayerStatus_Gameing);
	rTimeUnit.setOnTime(CTimeManager::Inst()->getCurrTime());
	rTimeUnit.setLastSaveTime(CTimeManager::Inst()->getCurrTime());
	// 新玩家
	if (0 == rTimeUnit.getLastOffTime())
	{
		setNewPlayerValue(pPlayer);
	}
	// 老玩家
	else
	{

	}

	CSceneJob::Inst()->createPlayer(pPlayer);

	pPlayer->refreshBaseProperty();
	pPlayer->refreshFightProperty();
	printf("%s%d%s\n", "*****************Load Complete:", pPlayer->getRoleID(), "*****************");
	LOG_INFO("Player load complete, %d", pPlayer->getRoleID());
	// 将玩家放入地图
	// 往客户端推送数据
}

/// 发送玩家基本信息通知
void CPropertyModule::sendPlayerBaseInfoNotify(CEntityPlayer* pPlayer)
{
	CPlayerBaseInfoNotify tNotify;

	tNotify.set_roleid(pPlayer->getRoleID());
	tNotify.set_entityid(pPlayer->getObjID());
	tNotify.set_lineid(pPlayer->getLineID());
	tNotify.set_mapid(pPlayer->getMapID());
	tNotify.set_mapindex(pPlayer->getMapIndex());
	tNotify.set_posx(pPlayer->getPosX());
	tNotify.set_posy(pPlayer->getPosY());
	tNotify.set_level(pPlayer->getLevel());
	tNotify.set_exp(pPlayer->getPropertyUnit().getExp());
	tNotify.set_viplevel(pPlayer->getVIPUnit().getVipLevel());
	tNotify.set_vipexp(pPlayer->getVIPUnit().getVipExp());

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIFY_PLAYER_BASE_INFO, &tNotify);
}

/// 发送玩家道具信息通知
void CPropertyModule::sendPlayerItemInfoNotify(CEntityPlayer* pPlayer)
{
	CPlayerItemInfoNotify tNotify;
	pPlayer->getItemUnit().getBag().createToPB(tNotify.mutable_bag());

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIFY_PLAYER_ITEM_INFO, &tNotify);
}

/// 发送玩家任务信息通知
void CPropertyModule::sendPlayerTaskInfoNotify(CEntityPlayer* pPlayer)
{
	CPlayerTaskInfoNotify tNotify;
	pPlayer->getTaskUnit().createToPB(tNotify.mutable_tasklist());

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIFY_PLAYER_TASK_INFO, &tNotify);
}

/// 玩家技能信息通知
void CPropertyModule::sendPlayerSkillInfoNotify(CEntityPlayer* pPlayer)
{

}

/// 玩家存盘
void CPropertyModule::savePlayer(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}


	pPlayer->setSaveStatus(0);
	savePlayerInfo(pPlayer);
	savePlayerBaseProperty(pPlayer);
}

void CPropertyModule::savePlayerInfo(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	time_t tCurrTime = CTimeManager::Inst()->getCurrTime();
	CDBModule::Inst()->pushDBTask(pPlayer->getRoleID(), emSessionType_SavePlayerInfo, pPlayer->getObjID(), 0,
		"call UpdatePlayerInfo(%d, %d,%lld,%d,%d,%d,%d,%d)", pPlayer->getRoleID(),
		pPlayer->getPropertyUnit().getLevel(), pPlayer->getPropertyUnit().getExp(),
		pPlayer->getVIPUnit().getVipLevel(), pPlayer->getVIPUnit().getVipExp(),
		pPlayer->getItemUnit().getMoney(), pPlayer->getItemUnit().getDiamond(), tCurrTime);
}

void CPropertyModule::savePlayerBaseProperty(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}
	PBSavePlayer tSavePlayer;
	pPlayer->getItemUnit().getBag().createToPB(tSavePlayer.mutable_bag());
	pPlayer->getItemUnit().getEquip().createToPB(tSavePlayer.mutable_equip());
	pPlayer->getTaskUnit().createToPB(tSavePlayer.mutable_task());
	
	CDBModule::Inst()->pushDBTask(pPlayer->getRoleID(), emSessionType_SavePlayerBaseProperty, pPlayer->getObjID(), 0, &tSavePlayer);
}
/// 玩家存盘完成
void CPropertyModule::onSavePlayerComplete(CEntityPlayer* pPlayer)
{
	// 没完全存盘就直接返回
	if (emSaveStatusAll !=pPlayer->getSaveStatus())
	{
		return;
	}
	// 退出状态
	if (pPlayer->getPlayerStauts() == emPlayerStatus_Exiting)
	{
		onPlayerLeaveGame(pPlayer);
	}
}

/// 玩家离开游戏
void CPropertyModule::playerLeaveGame(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}
	if (emPlayerStatus_Exiting == pPlayer->getPlayerStauts())
	{
		return;
	}

	byte bPlayerStatus = pPlayer->getPlayerStauts();
	// 将玩家置为下线状态
	pPlayer->setPlayerStauts(emPlayerStatus_Exiting);

	switch (bPlayerStatus)
	{
		case emPlayerStatus_Gameing:
		{
			// 玩家是在游戏状态，先保存
			savePlayer(pPlayer);
			break;
		}
		case emPlayerStatus_Loading:
		{
			// 直接离开游戏，不保存
			onPlayerLeaveGame(pPlayer);
			break;
		}
	}
}

/// 玩家离开游戏
void CPropertyModule::onPlayerLeaveGame(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	CSceneJob::Inst()->onPlayerLeaveGame(pPlayer);
	CObjPool::Inst()->free(pPlayer->getObjID());
}

/// 新玩家处理
void CPropertyModule::setNewPlayerValue(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}
	//
	CTplNewPlayerConfig* pTplConfig = CTplNewPlayerConfig::spConfig;
	if (NULL == pTplConfig)
	{
		return;
	}

	pPlayer->getItemUnit().insertItem(pTplConfig->mItemID, pTplConfig->mItemNum, MAX_NEW_PLAYER_ITEM);
	pPlayer->getPropertyUnit().setLevel(pTplConfig->mLevel);
	pPlayer->getVIPUnit().setVipLevel(pTplConfig->mVipLevel);
}

/// 创建实体后的处理
void CPropertyModule::onCreateEntity(CEntityCreator* pCreator, CEntity* pEntity)
{
	if (NULL == pCreator || NULL == pEntity)
	{
		return;
	}

	switch (pEntity->getEntityType())
	{
		case emEntityType_Item:
		{
			break;
		}
		default:
			break;
	}
}