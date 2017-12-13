#include "propertymodule.h"
#include "propertyunit.h"
#include "entityplayer.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"
#include "locallogjob.h"
#include "dbmessage.h"
#include "gameserver.h"
#include "dbmodule.h"
CPropertyModule::CPropertyModule()
:mSavePlayerTimer(1000)
{
	
}

/// 时间函数
void CPropertyModule::OnTimer(unsigned int nTickOffset)
{
	if (mSavePlayerTimer.elapse(nTickOffset))
	{
		time_t tTimeNow = CGameServer::Inst()->GetCurrTime();
		int nSavePlayerCount = 0;
		CSceneJob::PLAYER_LIST rPlayerList = CSceneJob::Inst()->getPlayerList();
		CSceneJob::PLAYER_LIST::iterator tPlayerIt = rPlayerList.begin();
		for (; tPlayerIt != rPlayerList.end(); ++tPlayerIt)
		{
			CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(tPlayerIt->second);
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
					SavePlayer(pPlayer);
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
			onMessageGMCommandRequest(pPlayer, pMessage);
			break;
		}
		default:
			break;
	}
}


// GM命令请求
void CPropertyModule::onMessageGMCommandRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);

	CGMCommandRequest* pGMCommandRequest = reinterpret_cast<CGMCommandRequest*>(pMessage);
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

/// 加载玩家信息
void CPropertyModule::onLoadPlayerInfo(CDBResponse& rResponse)
{
	CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(rResponse.mParam1);
	if (NULL == pPlayer)
	{
		return;
	}
	// 	select role_name, level, exp, vip_level, vip_exp, money, diamond from PlayerRole WHERE role_id=RoleID;
	
	rResponse.getString(pPlayer->getName(), PLAYER_NAME_LENGTH - 1);
	pPlayer->GetPropertyUnit().setLevel((byte)rResponse.getShort());
	pPlayer->GetPropertyUnit().setRoleExp(rResponse.getInt64());
	pPlayer->GetPropertyUnit().setVIPLevel(rResponse.getByte());
	pPlayer->GetPropertyUnit().SetVIPExp(rResponse.getInt());


	pPlayer->GetItemUnit().setMoney(rResponse.getInt());
	pPlayer->GetItemUnit().setDiamond(rResponse.getInt());

	pPlayer->setLoadStatusBit(emLoadStatus_Info);
}

/// 加载玩家基础属性
void CPropertyModule::onLoadPlayerBaseProperty(CDBResponse& rResponse)
{
	CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(rResponse.mParam1);
	if (NULL == pPlayer)
	{
		return;
	}
	pPlayer->setLoadStatusBit(emLoadStatus_BaseProperty);
}

/// 玩家属性加载完成
void CPropertyModule::onLoadComplete(CEntityPlayer* pPlayer)
{
	// 玩家属性是否加载完成
	if (NULL == pPlayer || emLoadStatusAll != pPlayer->getLoadStatus())
	{
		return;
	}

}


/// 玩家存盘
void CPropertyModule::SavePlayer(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}
	SavePlayerInfo(pPlayer);
	SavePlayerBaseProperty(pPlayer);
}

void CPropertyModule::SavePlayerInfo(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	CDBModule::Inst()->pushDBTask(pPlayer->getRoleID(), emSessionType_SavePlayerInfo, pPlayer->getObjID(), 0,
		"call UpdatePlayerInfo(%d,%lld,%d,%d,%d,%d)",
		pPlayer->GetPropertyUnit().getLevel(), pPlayer->GetPropertyUnit().getRoleExp(),
		pPlayer->GetPropertyUnit().getVIPLevel(), pPlayer->GetPropertyUnit().GetVIPExp(),
		pPlayer->GetItemUnit().getMoney(), pPlayer->GetItemUnit().getDiamond());
}

void CPropertyModule::SavePlayerBaseProperty(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}
}