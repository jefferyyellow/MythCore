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
void CPropertyModule::onCreatePlayer(CEntity* pEntity)
{

}

/// ����ʵ��
void CPropertyModule::onDestroyPlayer(CEntity* pEntity)
{

}

/// ʱ�亯��
void CPropertyModule::onTimer(unsigned int nTickOffset)
{
	if (mSavePlayerTimer.elapse(nTickOffset))
	{
		time_t tTimeNow = CGameServer::Inst()->GetCurrTime();
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

			// ��Ϸ����
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


// GM��������
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

/// ����뿪��Ϸ������
void CPropertyModule::onLeaveGameRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	// �������Ϊ����״̬
	pPlayer->setPlayerStauts(emPlayerStatus_Exiting);
	savePlayer(pPlayer);
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

		pPlayer->setPlayerStauts(emPlayerStatus_Exiting);
		savePlayer(pPlayer);
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
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(rResponse.mParam1));
	if (NULL == pPlayer)
	{
		return;
	}
	// 	select role_name, level, exp, vip_level, vip_exp, money, diamond from PlayerRole WHERE role_id=RoleID;
	
	rResponse.getString(pPlayer->getName(), PLAYER_NAME_LENGTH - 1);
	pPlayer->getPropertyUnit().setLevel((byte)rResponse.getShort());
	pPlayer->getPropertyUnit().setRoleExp(rResponse.getInt64());
	pPlayer->getPropertyUnit().setVIPLevel(rResponse.getByte());
	pPlayer->getPropertyUnit().SetVIPExp(rResponse.getInt());


	pPlayer->getItemUnit().setMoney(rResponse.getInt());
	pPlayer->getItemUnit().setDiamond(rResponse.getInt());

	pPlayer->setLoadStatusBit(emLoadStatus_Info);
	onLoadComplete(pPlayer);
}

/// ������һ�������
void CPropertyModule::onLoadPlayerBaseProperty(CDBResponse& rResponse)
{
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
	// ˢ������װ��������
	pPlayer->getItemUnit().getEquipList().refreshProperty();

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
	pPlayer->setPlayerStauts(emPlayerStatus_Gameing);
	// ����ҷ����ͼ
	// ֪ͨ����ģ������Ҵ���
	// ���ͻ�����������
}


/// ��Ҵ���
void CPropertyModule::savePlayer(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}
	savePlayerInfo(pPlayer);
	savePlayerBaseProperty(pPlayer);
}

void CPropertyModule::savePlayerInfo(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	CDBModule::Inst()->pushDBTask(pPlayer->getRoleID(), emSessionType_SavePlayerInfo, pPlayer->getObjID(), 0,
		"call UpdatePlayerInfo(%d, %d,%lld,%d,%d,%d,%d)", pPlayer->getRoleID(),
		pPlayer->getPropertyUnit().getLevel(), pPlayer->getPropertyUnit().getRoleExp(),
		pPlayer->getPropertyUnit().getVIPLevel(), pPlayer->getPropertyUnit().GetVIPExp(),
		pPlayer->getItemUnit().getMoney(), pPlayer->getItemUnit().getDiamond());

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
void CPropertyModule::onPlayerLeaveGame(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	CSceneJob::Inst()->onPlayerLeaveGame(pPlayer);
	CObjPool::Inst()->free(pPlayer->getObjID());
}