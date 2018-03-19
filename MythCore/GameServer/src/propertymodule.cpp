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
	dailyRefresh(pPlayer);
}

/// 销毁实体
void CPropertyModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

/// 时间函数
void CPropertyModule::onTimer(unsigned int nTickOffset)
{
	if (mSavePlayerTimer.elapse(nTickOffset))
	{
		time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
		int nSavePlayerCount = 0;
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
			if (pPlayer->getPlayerStauts() == emPlayerStatus_Gameing)
			{
				if (tTimeNow - pPlayer->getLastSaveTime() > 60)
				{
					printf("CPropertyModule::OnTimer\n");
					savePlayer(pPlayer);
					pPlayer->setLastSaveTime(tTimeNow);
					++nSavePlayerCount;
					if (nSavePlayerCount >= 50)
					{
						break;
					}
				}
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
		default:
			break;
	}
}


// GM命令请求
void CPropertyModule::onGMCommandRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);

	CGMCommandRequest* pGMCommandRequest = static_cast<CGMCommandRequest*>(pMessage);
	if (NULL == pGMCommandRequest)
	{
		return;
	}

	const std::string& rCommondString = pGMCommandRequest->commandstring();
	StrTokens tTokens = strSplit(rCommondString, " ");
	MYTH_ASSERT(tTokens.size() >= 1, return);

	const char* pCommandName = tTokens[0].c_str();
	tTokens.erase(tTokens.begin());
	mGMCmdManager.excuteCommand(pCommandName, tTokens, pPlayer);
}

/// 玩家离开游戏的请求
void CPropertyModule::onLeaveGameRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	// 将玩家置为下线状态
	pPlayer->setPlayerStauts(emPlayerStatus_Exiting);
	savePlayer(pPlayer);
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

		pPlayer->setPlayerStauts(emPlayerStatus_Exiting);
		savePlayer(pPlayer);
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
	// 	select role_name, level, exp, vip_level, vip_exp, money, diamond from PlayerRole WHERE role_id=RoleID;
	
	rResponse.getString(pPlayer->getName(), PLAYER_NAME_LENGTH - 1);
	pPlayer->getPropertyUnit().setLevel((byte)rResponse.getShort());
	pPlayer->getPropertyUnit().setExp(rResponse.getInt64());
	pPlayer->getPropertyUnit().setVipLevel(rResponse.getByte());
	pPlayer->getPropertyUnit().setVipExp(rResponse.getInt());


	pPlayer->getItemUnit().setMoney(rResponse.getInt());
	pPlayer->getItemUnit().setDiamond(rResponse.getInt());
	
	pPlayer->setLastOffTime(rResponse.getInt());

	pPlayer->setLoadStatusBit(emLoadStatus_Info);
	onLoadComplete(pPlayer);
}

/// 加载玩家基础属性
void CPropertyModule::onLoadPlayerBaseProperty(CDBResponse& rResponse)
{
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

	PBItemList tBag;
	tBag.ParseFromArray(rResponse.getValue(), rResponse.getLength());
	rResponse.next();
	pPlayer->getItemUnit().getBag().setFromPB(&tBag);

	PBItemList tEquip;
	tEquip.ParseFromArray(rResponse.getValue(), rResponse.getLength());
	rResponse.next();
	pPlayer->getItemUnit().getEquipList().setFromPB(&tEquip);
	// 刷新所以装备的属性
	pPlayer->getItemUnit().getEquipList().refreshProperty();

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
	pPlayer->setPlayerStauts(emPlayerStatus_Gameing);
	pPlayer->setOnTime(CTimeManager::Inst()->getCurrTime());

	// 新玩家
	if (0 == pPlayer->getLastOffTime())
	{
		setNewPlayerValue(pPlayer);
	}

	CSceneJob::Inst()->createPlayer(pPlayer);

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
	tNotify.set_viplevel(pPlayer->getPropertyUnit().getVipLevel());
	tNotify.set_vipexp(pPlayer->getPropertyUnit().getVipExp());

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
		pPlayer->getPropertyUnit().getVipLevel(), pPlayer->getPropertyUnit().getVipExp(),
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
	pPlayer->getItemUnit().getEquipList().createToPB(tSavePlayer.mutable_equip());
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
void CPropertyModule::onPlayerLeaveGame(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	CSceneJob::Inst()->onPlayerLeaveGame(pPlayer);
	CObjPool::Inst()->free(pPlayer->getObjID());
}

/// 每日刷新
void CPropertyModule::dailyRefresh(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	/// 如果上次下线的时间在今天早上以后了，那就是同一天的多次登录了
	if (pPlayer->getLastOffTime() >= CSceneJob::Inst()->getMorningTime())
	{
		return;
	}

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
	pPlayer->getPropertyUnit().setVipLevel(pTplConfig->mVipLevel);
}
