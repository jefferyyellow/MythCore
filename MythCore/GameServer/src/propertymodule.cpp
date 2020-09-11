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
#include "mailmodule.h"
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


	CShareObjPoolImp::PlayerEntityPool& rPlayerEntityPool = CObjPool::Inst()->getShareObjPoolImp()->mPlayerEntityPool;
	CEntityPlayer* pPlayer = rPlayerEntityPool.begin();
	for (; pPlayer != NULL; pPlayer = rPlayerEntityPool.next(pPlayer))
	{
		if (emPlayerStatus_Gameing != pPlayer->getPlayerStauts())
		{
			continue;
		}
		pPlayer->getTimeUnit().getTimerList().update(tTickCount);
		tArrayPlayer[nPlayerNum] = pPlayer;
		++nPlayerNum;
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

			savePlayer(*tArrayPlayer[i]);
			tArrayPlayer[i]->getTimeUnit().setLastSaveTime(tTimeNow);
			++nSavePlayerCount;
			if (nSavePlayerCount >= 50)
			{
				break;
			}
		}
	}
}

/// ���������ļ�
void CPropertyModule::onLoadConfig()
{

}

void CPropertyModule::onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_GM_COMMAND:
		{
			onGMCommandRequest(rPlayer, pMessage);
			break;
		}
		case ID_C2S_REQUEST_LEAVE_GAME:
		{
			onLeaveGameRequest(rPlayer, pMessage);
			break;
		}
		case ID_C2S_REQUEST_GET_PLAYER_PROPERTY:
		{
			rPlayer.onGetPlayerPropertyRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_HEART_BEAT:
		{
			rPlayer.getTimeUnit().onHeartBeatRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_GET_MAIL_LIST:
		{
			rPlayer.getInteractiveUnit().onGetMailListRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_READ_MAIL:
		{
			rPlayer.getInteractiveUnit().onReadMailRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_GET_MAIL_ATTACHMENT:
		{
			rPlayer.getInteractiveUnit().onGetMailAttachmentRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_DELETE_MAIL:
		{
			rPlayer.getInteractiveUnit().onDeleteMailRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_GET_MAIL_DETAIL:
		{
			rPlayer.getInteractiveUnit().onGetMailDetailRequest(pMessage);
			break;
		}
		default:
			break;
	}
}


// GM��������
void CPropertyModule::onGMCommandRequest(CEntityPlayer& rPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);

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
	mGMCmdManager.excuteCommand(acCommandName, tTokens, rPlayer);
}

/// ����뿪��Ϸ������
void CPropertyModule::onLeaveGameRequest(CEntityPlayer& rPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	// �������Ϊ����״̬
	playerLeaveGame(rPlayer, EmLeaveReason_PlayerRequest);
}

/// �߳��������
void CPropertyModule::kickAllPlayer()
{
	int nCount = 0;
	CShareObjPoolImp::PlayerEntityPool& rPlayerEntityPool = CObjPool::Inst()->getShareObjPoolImp()->mPlayerEntityPool;
	CEntityPlayer* pPlayer = rPlayerEntityPool.begin();
	for (; pPlayer != NULL; pPlayer = rPlayerEntityPool.next(pPlayer))
	{
		if (pPlayer->getPlayerStauts() == emPlayerStatus_Exiting)
		{
			continue;
		}

		playerLeaveGame(*pPlayer, EmLeaveReason_ServerShutDown);
		++nCount;
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

	// ���ػ�����ʱ�򣬵�ʱ������Ѿ����ߣ��ڴ��Ѿ�������
	if (pPlayer->getRoleID() != rResponse.mPlayerID)
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

	// ���ػ�����ʱ�򣬵�ʱ������Ѿ����ߣ��ڴ��Ѿ�������
	if (pPlayer->getRoleID() != rResponse.mPlayerID)
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
		// �ճ�ˢ��
		pPlayer->dailyRefresh(true);
	}

	CSceneJob::Inst()->createPlayer(pPlayer);


	pPlayer->refreshBaseProperty();
	pPlayer->refreshFightProperty();
	printf("%s%d%s\n", "*****************Load Complete:", pPlayer->getRoleID(), "*****************");
	LOG_INFO("Player load complete, %d", pPlayer->getRoleID());
	// ����ҷ����ͼ
	// ���ͻ�����������

	// ��������ʼ�
	CMailModule::Inst()->loadPlayerMail(pPlayer->getRoleID());
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
void CPropertyModule::savePlayer(CEntityPlayer& rPlayer)
{
	rPlayer.setSaveStatus(0);
	savePlayerInfo(rPlayer);
	savePlayerBaseProperty(rPlayer);
}

void CPropertyModule::savePlayerInfo(CEntityPlayer& rPlayer)
{
	time_t tCurrTime = CTimeManager::Inst()->getCurrTime();
	CDBModule::Inst()->pushDBTask(rPlayer.getRoleID(), emSessionType_SavePlayerInfo, rPlayer.getObjID(), 0,
		"call UpdatePlayerInfo(%d, %d,%lld,%d,%d,%d,%d,%d)", rPlayer.getRoleID(),
		rPlayer.getPropertyUnit().getLevel(), rPlayer.getPropertyUnit().getExp(),
		rPlayer.getVIPUnit().getVipLevel(), rPlayer.getVIPUnit().getVipExp(),
		rPlayer.getItemUnit().getMoney(), rPlayer.getItemUnit().getDiamond(), tCurrTime);
}

void CPropertyModule::savePlayerBaseProperty(CEntityPlayer& rPlayer)
{
	PBSavePlayer tSavePlayer;
	rPlayer.getItemUnit().getBag().createToPB(tSavePlayer.mutable_bag());
	rPlayer.getItemUnit().getEquip().createToPB(tSavePlayer.mutable_equip());
	rPlayer.getTaskUnit().createToPB(tSavePlayer.mutable_task());
	
	CDBModule::Inst()->pushDBTask(rPlayer.getRoleID(), emSessionType_SavePlayerBaseProperty, rPlayer.getObjID(), 0, &tSavePlayer);
}
/// ��Ҵ������
void CPropertyModule::onSavePlayerComplete(CEntityPlayer& rPlayer)
{
	// û��ȫ���̾�ֱ�ӷ���
	if (emSaveStatusAll != rPlayer.getSaveStatus())
	{
		return;
	}
	// �˳�״̬
	if (rPlayer.getPlayerStauts() == emPlayerStatus_Exiting)
	{
		destroyPlayer(rPlayer);
	}
}

/// ����뿪��Ϸ
void CPropertyModule::playerLeaveGame(CEntityPlayer& rPlayer, EmLeaveReason eReason)
{
	if (emPlayerStatus_Exiting == rPlayer.getPlayerStauts())
	{
		return;
	}

	LOG_INFO("player leave game, player status: %d, reason: %d", rPlayer.getPlayerStauts(), eReason);

	byte bPlayerStatus = rPlayer.getPlayerStauts();
	// �������Ϊ����״̬
	rPlayer.setPlayerStauts(emPlayerStatus_Exiting);

	switch (bPlayerStatus)
	{
		case emPlayerStatus_Gameing:
		{
			// ���������Ϸ״̬���ȱ���
			savePlayer(rPlayer);
			break;
		}
		case emPlayerStatus_Loading:
		{
			// ֱ���뿪��Ϸ��������
			destroyPlayer(rPlayer);
			break;
		}
	}
}

/// ����뿪��Ϸ
void CPropertyModule::destroyPlayer(CEntityPlayer& rPlayer)
{
	CSceneJob::Inst()->destroyPlayerObject(rPlayer);
	CObjPool::Inst()->free(rPlayer.getObjID());
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

	pPlayer->getItemUnit().insertAllItem(pTplConfig->mItemID, pTplConfig->mItemNum, MAX_NEW_PLAYER_ITEM);
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