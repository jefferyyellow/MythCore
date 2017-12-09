#include "loginmodule.h"
#include "loginmessage.hxx.pb.h"
#include "dbmessage.h"
#include "internalmsgpool.h"
#include "gameserver.h"
#include "entityplayer.h"
#include "objpool.h"
#include "timemanager.h"
#include "loginplayer.h"
#include "dbmodule.h"
CLoginModule::CLoginModule()
:mLoginCheckTime(1000)
{

}


CLoginModule::~CLoginModule()
{

}

/// 时间函数
void CLoginModule::OnTimer(unsigned int nTickOffset)
{
	if (mLoginCheckTime.elapse(nTickOffset))
	{
		time_t tTimeNow = CGameServer::Inst()->GetCurrTime();
		LOGIN_LIST::iterator it = mLoginList.begin();
		for (; it != mLoginList.end(); )
		{
			int nObjID = it->second;
			CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
			if (NULL == pLoginPlayer)
			{
				it = mLoginList.erase(it);
				continue;
			}
			// 超时
			if (pLoginPlayer->elapse(nTickOffset))
			{
				CObjPool::Inst()->free(nObjID);
				it = mLoginList.erase(it);
				continue;
			}

			++ it;
		}
	}
}

void CLoginModule::onClientMessage(CExchangeHead& rExchangeHead, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pMessage)
	{
		return;
	}
	CLoginPlayer* pLoginPlayer = NULL;
	LOGIN_LIST::iterator it = mLoginList.find(rExchangeHead.mSocketIndex);
	if (it == mLoginList.end())
	{
		pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->allocObj(emObjType_LoginPlayer));
		// 分配失败
		if (NULL == pLoginPlayer)
		{
			return;
		}
		std::pair<LOGIN_LIST::iterator, bool> ret = mLoginList.insert(LOGIN_LIST::value_type(rExchangeHead.mSocketIndex, pLoginPlayer->getObjID()));
		// 插入列表失败
		if (!ret.second)
		{
			CObjPool::Inst()->free(pLoginPlayer->getObjID());
			return;
		}
		pLoginPlayer->getExchangeHead() = rExchangeHead;
		it = ret.first;
	}
	else
	{
		pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
	}
	if (NULL == pLoginPlayer)
	{
		return;
	}

	if (nMessageID == ID_C2S_REQUEST_ENTER_SCENE)
	{
		processWaitEnterGame(pLoginPlayer, pMessage);
		// 完成使命，释放掉
		CObjPool::Inst()->free(pLoginPlayer->getObjID());
	}
	else
	{
		pLoginPlayer->setClientMessage(pMessage);
		pLoginPlayer->setClientMessageID(nMessageID);
		pLoginPlayer->checkState();
	}
}

void CLoginModule::OnDBMessage(CDBResponse* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	int nLoginPlayerObjID  = pMsg->mParam1;
	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nLoginPlayerObjID));
	if (NULL == pLoginPlayer)
	{
		return;
	}
	pLoginPlayer->setDBMessage(pMsg);
	pLoginPlayer->setDBSessionType((EmSessionType)pMsg->mSessionType);
	pLoginPlayer->checkState();
	pLoginPlayer->setDBMessage(NULL);
	pLoginPlayer->setDBSessionType(emSessionType_None);
}

/// 处理等待玩家进入游戏
void CLoginModule::processWaitEnterGame(CLoginPlayer* pLoginPlayer, Message* pMessage)
{
	if (NULL == pLoginPlayer || NULL == pMessage)
	{
		return;
	}
	if (pLoginPlayer->getLoginState() != emLoginState_LoginComplete)
	{
		return;
	}
	CEnterSceneRequest* pEnterSceneRequest = reinterpret_cast<CEnterSceneRequest*>(pMessage);
	if (NULL == pEnterSceneRequest)
	{
		return;
	}

	if (pLoginPlayer->getServerID() != pEnterSceneRequest->serverid()
	|| pLoginPlayer->getChannelID() != pEnterSceneRequest->channelid()
	|| pLoginPlayer->getAccountID() != pEnterSceneRequest->accountid()
	|| pLoginPlayer->getRoleID() != pEnterSceneRequest->roleid())
	{
		return;
	}

	CEntityPlayer* pNewPlayer = reinterpret_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	if (NULL == pNewPlayer)
	{
		return;
	}

	pNewPlayer->setPlayerStauts(emPlayerStatus_Loading);
	pNewPlayer->setRoleID(pEnterSceneRequest->roleid());
	pNewPlayer->GetExhangeHead() = pLoginPlayer->getExchangeHead();


	bool bResult = CSceneJob::Inst()->onPlayerLogin(pNewPlayer);
	if (!bResult)
	{
		CObjPool::Inst()->free(pNewPlayer->getObjID());
		return;
	}

	CDBModule::Inst()->pushDBTask(pLoginPlayer->getRoleID(), emSessionType_LoadPlayerInfo, pLoginPlayer->getObjID(), 0, "call LoadPlayerInfo(%d)", pLoginPlayer->getRoleID());
	CDBModule::Inst()->pushDBTask(pLoginPlayer->getRoleID(), emSessionType_LoadPlayerInfo, pLoginPlayer->getObjID(), 0, "call LoadPlayerBaseProperty(%d)", pLoginPlayer->getRoleID());

	CEnterSceneResponse tEnterSceneResponse;
	tEnterSceneResponse.set_result(0);
	CSceneJob::Inst()->send2Player(pNewPlayer->GetExhangeHead(), ID_S2C_RESPONSE_ENTER_SCENE, &tEnterSceneResponse);
}
