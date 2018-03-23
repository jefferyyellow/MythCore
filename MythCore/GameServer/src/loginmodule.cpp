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
	mCheckResult = false;
}


CLoginModule::~CLoginModule()
{

}

/// 启动服务器
void CLoginModule::onLaunchServer()
{
	loadAllocateRoleId();
}

/// 启动完成检查
bool CLoginModule::onCheckLaunch()
{
	return mCheckResult;
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
void CLoginModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
void CLoginModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

/// 时间函数
void CLoginModule::onTimer(unsigned int nTickOffset)
{
	if (mLoginCheckTime.elapse(nTickOffset))
	{
		time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
		LOGIN_LIST::iterator it = mLoginList.begin();
		for (; it != mLoginList.end(); )
		{
			int nObjID = it->second;
			CLoginPlayer* pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
			if (NULL == pLoginPlayer)
			{
				it = mLoginList.erase(it);
				continue;
			}

			bool bOverTime = pLoginPlayer->elapse(1);
			// 如果是删除状态或者超时
			if (emLoginDelState_None != pLoginPlayer->GetDelState() || bOverTime)
			{
				removeVerifyPlayer(pLoginPlayer->getChannelID(), pLoginPlayer->getServerID(), pLoginPlayer->getAccountID());
				it = mLoginList.erase(it);
				CObjPool::Inst()->free(nObjID);
				// 如果登录完成了就不能断开连接
				if (emLoginDelState_Complete != pLoginPlayer->GetDelState())
				{
					CSceneJob::Inst()->disconnectPlayer(pLoginPlayer->getExchangeHead());
				}
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

	printf("pLoginPlayer: %d", pLoginPlayer->getObjID());

	// 如果是删除状态了，直接不处理
	if (emLoginDelState_None != pLoginPlayer->GetDelState())
	{
		return;
	}

	pLoginPlayer->setClientMessage(pMessage);
	pLoginPlayer->setClientMessageID(nMessageID);
	pLoginPlayer->checkState();
	pLoginPlayer->setClientMessage(NULL);
	pLoginPlayer->setClientMessageID(0);
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

	// 如果是删除状态了，直接不处理
	if (emLoginDelState_None != pLoginPlayer->GetDelState())
	{
		return;
	}

	printf("OnDBMessage: %d, %s\n", nLoginPlayerObjID, pLoginPlayer->getAccountName());
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


}

///  一个Socket断开
void CLoginModule::onSocketDisconnect(int nSocketIndex)
{
	LOGIN_LIST::iterator it = mLoginList.find(nSocketIndex);
	if (it != mLoginList.end())
	{
		CLoginPlayer* pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
		if (NULL != pLoginPlayer)
		{
			removeVerifyPlayer(pLoginPlayer->getChannelID(), pLoginPlayer->getServerID(), pLoginPlayer->getAccountID());
		}
		// 完成使命，释放掉
		CObjPool::Inst()->free(it->second);
		mLoginList.erase(nSocketIndex);
	}
}

/// 加载数据库里分配角色ID的字段
void CLoginModule::loadAllocateRoleId()
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_LoadAllocateRoleId, 0, 0,"select * from AllocateRoleId order by server_id asc limit %d", BAT_LOAD_ROLE_ID_NUM);
}

/// 加载数据库里分配角色ID的处理
void CLoginModule::onLoadAllocateRoleId(CDBResponse& rResponse)
{
	int nRow = rResponse.mRowNum;
	int nMaxServerId = 0;
	for (int i = 0; i < nRow; ++ i)
	{
		int nServerID = rResponse.getInt();
		unsigned int nRoleID = rResponse.getInt();
		setAllocateRoleId(nServerID, nRoleID);
		nMaxServerId = nServerID;
	}

	// 还没加载完成
	if (nRow >= BAT_LOAD_ROLE_ID_NUM)
	{
		CDBModule::Inst()->pushDBTask(0, emSessionType_LoadAllocateRoleId, 0, 0, "select * from AllocateRoleId where server_id > %d order by server_id asc limit %d", nMaxServerId, BAT_LOAD_ROLE_ID_NUM);
	}
	// 已经加载完了
	else
	{
		int nMaxRoleID = getAllocateRoleId(CGameServer::Inst()->getServerID());
		if (0 == nMaxRoleID)
		{
			int nBeginRoleID = getBeginRoleID(CGameServer::Inst()->getServerID());
			if (nBeginRoleID == 0)
			{
				LOG_ERROR("default", "begin role id is zero, server id: %d", CGameServer::Inst()->getServerID());
			}
			else
			{
				setAllocateRoleId(CGameServer::Inst()->getServerID(), nBeginRoleID);
				CDBModule::Inst()->pushDBTask(0, emSessionType_UpdateAllocateRoleId, 0, 0, 
				"insert into AllocateRoleId (server_id,max_role_id) values(%d, %d)", CGameServer::Inst()->getServerID(), nBeginRoleID);
			}
		}
	}
	mCheckResult = true;
}

unsigned int CLoginModule::allocateRoleID(int nServerId)
{
	int nMaxRoleId = getAllocateRoleId(nServerId);
	if(0 == nMaxRoleId)
	{
		return 0;
	}

	if (nMaxRoleId >= MAX_SERVER_ID * nServerId)
	{
		return 0;
	}

	setAllocateRoleId(nServerId, nMaxRoleId + 1);
	CDBModule::Inst()->pushDBTask(0, emSessionType_UpdateAllocateRoleId, 0, 0,
		"update AllocateRoleId set max_role_id=%d where server_id=%d", nMaxRoleId + 1, nServerId);
	return nMaxRoleId;
}

/// 增加正在校验的玩家
bool CLoginModule::addVerifyPlayer(short nChannelId, short nServerId, int nAccountId, int nLoginObjId)
{
	uint64 nLoginKey = MAKE_LOGIN_KEY((uint64)nAccountId, (uint64)nChannelId, (uint64)nServerId);
	std::pair<VERIFY_LIST::iterator, bool> tRet = mVerifyList.insert(
		VERIFY_LIST::value_type(nLoginKey, nLoginObjId));
	if (tRet.second)
	{
		return true;
	}
	return false;
}

/// 是否是正在校验的玩家
bool CLoginModule::checkVerifyPlayer(short nChannelId, short nServerId, int nAccountId)
{
	uint64 nLoginKey = MAKE_LOGIN_KEY((uint64)nAccountId, (uint64)nChannelId, (uint64)nServerId);
	VERIFY_LIST::iterator it = mVerifyList.find(nLoginKey);
	if (it != mVerifyList.end())
	{
		return true;
	}

	return false;
}

/// 删除正在校验的玩家
void CLoginModule::removeVerifyPlayer(short nChannelId, short nServerId, int nAccountId)
{
	uint64 nLoginKey = MAKE_LOGIN_KEY((uint64)nAccountId, (uint64)nChannelId, (uint64)nServerId);
	mVerifyList.erase(nLoginKey);
}