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

/// ����������
void CPropertyModule::onLaunchServer()
{
}

/// ������ɼ��
bool CPropertyModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CPropertyModule::onLaunchComplete()
{

}

/// �˳�������
void CPropertyModule::onExitServer()
{
	kickAllPlayer();
}

/// ��һ�쵽��
void CPropertyModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CPropertyModule::onNewWeekCome()
{

}

/// ����ʵ��
void CPropertyModule::onCreatePlayer(CEntityPlayer* pPlayer)
{
	
}

/// ����ʵ��
void CPropertyModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

/// ʱ�亯��
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
		// ��Ϸ����
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


// GM��������
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

/// ����뿪��Ϸ������
void CPropertyModule::onLeaveGameRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pPlayer && NULL != pMessage, return);
	// �������Ϊ����״̬
	playerLeaveGame(pPlayer);
}

/// �߳��������
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
		// ÿ��30��
		if (nCount >= 30)
		{
			break;
		}
	}
}

/// ���������Ϣ
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

	// �Ѿ������ˣ�����������飬ǰ��ʱ��ͳ����˼������ε�������в��������е���������
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

/// ������һ�������
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

	// �Ѿ������ˣ�����������飬ǰ��ʱ��ͳ����˼������ε�������в��������е���������
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
	// ˢ������װ��������
	pPlayer->getItemUnit().getEquip().refreshProperty();

	PBTaskList tTaskList;
	tTaskList.ParseFromArray(rResponse.getValue(), rResponse.getLength());
	rResponse.next();
	pPlayer->getTaskUnit().setFromPB(&tTaskList);

	pPlayer->setLoadStatusBit(emLoadStatus_BaseProperty);
	onLoadComplete(pPlayer);
}

/// ������Լ������
void CPropertyModule::onLoadComplete(CEntityPlayer* pPlayer)
{
	// ��������Ƿ�������
	if (NULL == pPlayer || emLoadStatusAll != pPlayer->getLoadStatus())
	{
		return;
	}
	// ��¼���ɾ��
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
	// �����
	if (0 == rTimeUnit.getLastOffTime())
	{
		setNewPlayerValue(pPlayer);
	}
	// �����
	else
	{

	}

	CSceneJob::Inst()->createPlayer(pPlayer);

	pPlayer->refreshBaseProperty();
	pPlayer->refreshFightProperty();
	printf("%s%d%s\n", "*****************Load Complete:", pPlayer->getRoleID(), "*****************");
	LOG_INFO("Player load complete, %d", pPlayer->getRoleID());
	// ����ҷ����ͼ
	// ���ͻ�����������
}

/// ������һ�����Ϣ֪ͨ
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

/// ������ҵ�����Ϣ֪ͨ
void CPropertyModule::sendPlayerItemInfoNotify(CEntityPlayer* pPlayer)
{
	CPlayerItemInfoNotify tNotify;
	pPlayer->getItemUnit().getBag().createToPB(tNotify.mutable_bag());

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIFY_PLAYER_ITEM_INFO, &tNotify);
}

/// �������������Ϣ֪ͨ
void CPropertyModule::sendPlayerTaskInfoNotify(CEntityPlayer* pPlayer)
{
	CPlayerTaskInfoNotify tNotify;
	pPlayer->getTaskUnit().createToPB(tNotify.mutable_tasklist());

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIFY_PLAYER_TASK_INFO, &tNotify);
}

/// ��Ҽ�����Ϣ֪ͨ
void CPropertyModule::sendPlayerSkillInfoNotify(CEntityPlayer* pPlayer)
{

}

/// ��Ҵ���
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
/// ��Ҵ������
void CPropertyModule::onSavePlayerComplete(CEntityPlayer* pPlayer)
{
	// û��ȫ���̾�ֱ�ӷ���
	if (emSaveStatusAll !=pPlayer->getSaveStatus())
	{
		return;
	}
	// �˳�״̬
	if (pPlayer->getPlayerStauts() == emPlayerStatus_Exiting)
	{
		onPlayerLeaveGame(pPlayer);
	}
}

/// ����뿪��Ϸ
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
	// �������Ϊ����״̬
	pPlayer->setPlayerStauts(emPlayerStatus_Exiting);

	switch (bPlayerStatus)
	{
		case emPlayerStatus_Gameing:
		{
			// ���������Ϸ״̬���ȱ���
			savePlayer(pPlayer);
			break;
		}
		case emPlayerStatus_Loading:
		{
			// ֱ���뿪��Ϸ��������
			onPlayerLeaveGame(pPlayer);
			break;
		}
	}
}

/// ����뿪��Ϸ
void CPropertyModule::onPlayerLeaveGame(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	CSceneJob::Inst()->onPlayerLeaveGame(pPlayer);
	CObjPool::Inst()->free(pPlayer->getObjID());
}

/// ����Ҵ���
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

/// ����ʵ���Ĵ���
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