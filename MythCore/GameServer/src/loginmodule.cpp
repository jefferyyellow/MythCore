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

/// 启动服务器
void CLoginModule::onLaunchServer()
{

}

/// 启动完成检查
bool CLoginModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CLoginModule::onLaunchComplete()
{

}

/// 退出服务器
void CLoginModule::onExitServer()
{

}

/// 新一天到来
void CLoginModule::onNewDayCome()
{

}

/// 新一周到来
void CLoginModule::onNewWeekCome()
{

}

/// 建立实体
void CLoginModule::onCreatePlayer(CEntity* pEntity)
{

}

/// 销毁实体
void CLoginModule::onDestroyPlayer(CEntity* pEntity)
{

}

/// 时间函数
void CLoginModule::onTimer(unsigned int nTickOffset)
{
	if (mLoginCheckTime.elapse(nTickOffset))
	{
		time_t tTimeNow = CGameServer::Inst()->GetCurrTime();
		LOGIN_LIST::iterator it = mLoginList.begin();
		for (; it != mLoginList.end(); )
		{
			int nObjID = it->second;
			CLoginPlayer* pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
			LOGIN_LIST::iterator itOld = it;
			if (NULL == pLoginPlayer)
			{
				++ it;
				mLoginList.erase(itOld);
				continue;
			}
			// 超时
			if (pLoginPlayer->elapse(1))
			{
				printf("\n***************\n");
				++ it;
				mLoginList.erase(itOld);
				CObjPool::Inst()->free(nObjID);
				CSceneJob::Inst()->disconnectPlayer(pLoginPlayer->getExchangeHead());
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
		pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->allocObj(emObjType_LoginPlayer));
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
		pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
	}
	if (NULL == pLoginPlayer)
	{
		return;
	}

	if (nMessageID == ID_C2S_REQUEST_ENTER_SCENE)
	{
		processWaitEnterGame(pLoginPlayer, pMessage);
		mLoginList.erase(rExchangeHead.mSocketIndex);
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
	CLoginPlayer* pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nLoginPlayerObjID));
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
	CEnterSceneRequest* pEnterSceneRequest = static_cast<CEnterSceneRequest*>(pMessage);
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

	CEntityPlayer* pPlayer = CSceneJob::Inst()->getPlayerByRoleID(pEnterSceneRequest->roleid());
	if (NULL != pPlayer)
	{
		printf("Kick out by other: %d\n", pEnterSceneRequest->roleid());
		// 如果加入不了到列表，就踢不下线
		if (!CSceneJob::Inst()->addPlayerSocketIndex(pLoginPlayer->getExchangeHead().mSocketIndex, pPlayer->getObjID()))
		{
			CSceneJob::Inst()->disconnectPlayer(pLoginPlayer->getExchangeHead());
			return;
		}
		// 如果已经是下线状态，改成正常的游戏状态
		if (pPlayer->getPlayerStauts() == emPlayerStatus_Exiting)
		{
			pPlayer->setPlayerStauts(emPlayerStatus_Gameing);
		}
		// 将原来的玩家下线
		CSceneJob::Inst()->disconnectPlayer(pPlayer);

		// 换成新的玩家socket信息
		pPlayer->GetExhangeHead() = pLoginPlayer->getExchangeHead();
		CEnterSceneResponse tEnterSceneResponse;
		tEnterSceneResponse.set_result(0);
		CSceneJob::Inst()->send2Player(pPlayer->GetExhangeHead(), ID_S2C_RESPONSE_ENTER_SCENE, &tEnterSceneResponse);
	}
	else
	{
		printf("new player login: %d", pEnterSceneRequest->roleid());
		CEntityPlayer* pNewPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
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

		CDBModule::Inst()->pushDBTask(pLoginPlayer->getRoleID(), emSessionType_LoadPlayerInfo, pNewPlayer->getObjID(), 0, "call LoadPlayerInfo(%d)", pLoginPlayer->getRoleID());
		CDBModule::Inst()->pushDBTask(pLoginPlayer->getRoleID(), emSessionType_LoadPlayerBaseProperty, pNewPlayer->getObjID(), 0, "call LoadPlayerBaseProperty(%d)", pLoginPlayer->getRoleID());

		CEnterSceneResponse tEnterSceneResponse;
		tEnterSceneResponse.set_result(0);
		CSceneJob::Inst()->send2Player(pNewPlayer->GetExhangeHead(), ID_S2C_RESPONSE_ENTER_SCENE, &tEnterSceneResponse);
	}
}

///  一个Socket断开
void CLoginModule::onSocketDisconnect(int nSocketIndex)
{
	LOGIN_LIST::iterator it = mLoginList.find(nSocketIndex);
	if (it != mLoginList.end())
	{
		mLoginList.erase(nSocketIndex);
		// 完成使命，释放掉
		CObjPool::Inst()->free(it->second);
	}
}