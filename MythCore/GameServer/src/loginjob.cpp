#include "loginjob.h"
#include "internalmsgpool.h"
#include "loginplayer.h"
#include "objpool.h"
#include "clientsocketjob.h"
#include "serverutility.h"
#include "timemanager.h"
#include "locallogjob.h"
#include "gameserver.h"
#include "dbmodule.h"
CLoginJob::CLoginJob()
	:mLoginCheckTime(1000)
{

}

CLoginJob::~CLoginJob()
{

}
void CLoginJob::init()
{

}

void CLoginJob::clear()
{

}

void CLoginJob::doing(int uParam)
{
	int nCount = 0;
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}

		switch (pIMMsg->getMsgID())
		{
			case IM_REQUEST_PLAYER_LOGIN:
			{
				OnIMPlayerLoginRequest(pIMMsg);
				break;
			}
		}
		delete pIMMsg;
		++nCount;
		if (nCount > 1000)
		{
			break;
		}
	}
	onTimer();
}

void CLoginJob::OnIMPlayerLoginRequest(CInternalMsg* pIMMsg)
{
	if (NULL == pIMMsg)
	{
		return;
	}

	CIMPlayerLoginRequest* pRequest = (CIMPlayerLoginRequest*)pIMMsg;
	short nSocketIndex = pRequest->mSocketIndex;
	
	CLoginPlayer* pLoginPlayer = NULL;
	LOGIN_LIST::iterator it = mLoginList.find(nSocketIndex);
	if (it == mLoginList.end())
	{
		pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->allocObj(emObjType_LoginPlayer));
		// 分配失败
		if (NULL == pLoginPlayer)
		{
			return;
		}
		std::pair<LOGIN_LIST::iterator, bool> ret = 
			mLoginList.insert(LOGIN_LIST::value_type(nSocketIndex, pLoginPlayer->getObjID()));
		// 插入列表失败
		if (!ret.second)
		{
			CObjPool::Inst()->free(pLoginPlayer->getObjID());
			return;
		}
		CExchangeHead& rExchangeHead = pLoginPlayer->getExchangeHead();
		rExchangeHead.mSocketTime = pRequest->mSocketTime;
		rExchangeHead.mSocketIndex = nSocketIndex;
		pLoginPlayer->SetMsgQueue((CPlayerMsgQueue*)(pRequest->mMsgQueue));
	}
	else
	{
		pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
		if (NULL != pLoginPlayer)
		{
			CExchangeHead& rExchangeHead = pLoginPlayer->getExchangeHead();
			rExchangeHead.mSocketTime = pRequest->mSocketTime;
			rExchangeHead.mSocketIndex = nSocketIndex;
			pLoginPlayer->SetMsgQueue((CPlayerMsgQueue*)(pRequest->mMsgQueue));
		}
	}
}

void CLoginJob::onTimer()
{
	uint64 nNowTick = getTickCount();
	int nElapseTime = (int)(nNowTick - mLastTimerTick);
	if (nElapseTime < 100)
	{
		return;
	}

	checkLoginList(nElapseTime);
	mLastTimerTick = nNowTick;

	LOGIN_LIST::iterator it = mLoginList.begin();
	for (; it != mLoginList.end(); ++ it)
	{
		CLoginPlayer* pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
		if (NULL == pLoginPlayer)
		{
			continue;
		}
		if (emLoginDelState_None != pLoginPlayer->getDelState())
		{
			continue;
		}
		pLoginPlayer->onTimer();
	}
}

void CLoginJob::checkLoginList(int nElapseTime)
{
	if (mLoginCheckTime.elapse(nElapseTime))
	{
		time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
		LOGIN_LIST::iterator it = mLoginList.begin();
		LOGIN_LIST::iterator itTmp = mLoginList.end();
		for (; it != mLoginList.end();)
		{
			int nObjID = it->second;
			CLoginPlayer* pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
			if (NULL == pLoginPlayer)
			{
				itTmp = it;
				++it;
				mLoginList.erase(itTmp);
				continue;
			}

			bool bOverTime = pLoginPlayer->elapse(1);
			// 如果是删除状态或者超时
			if (emLoginDelState_None != pLoginPlayer->getDelState() || bOverTime)
			{
				// 如果已经在加载状态了，那肯定有对应的CEntityPlayer，不能存盘，直接删除对应的实体和socket
				if (emLoginState_RoleLoading == pLoginPlayer->getLoginState())
				{
					//CEntityPlayer* pPlayer = CSceneJob::Inst()->getPlayerBySocketIndex(pLoginPlayer->getExchangeHead().mSocketIndex);
					//if (NULL != pPlayer)
					//{
					//	CPropertyModule::Inst()->playerLeaveGame(*pPlayer, EmLeaveReason_LoadTimeOut);
					//}
				}
				printf("delete login player, State: %d, DelState: %d, OverTime: %d", pLoginPlayer->getLoginState(), pLoginPlayer->getDelState(), bOverTime);

				itTmp = it;
				++it;
				mLoginList.erase(itTmp);

				CObjPool::Inst()->free(nObjID);
				// 如果登录完成了就不能断开连接
				if (emLoginDelState_Complete != pLoginPlayer->getDelState())
				{
					//CSceneJob::Inst()->disconnectPlayer(pLoginPlayer->getExchangeHead());
				}
				continue;
			}

			++it;
		}
	}
}

///  一个Socket断开
void CLoginJob::onSocketDisconnect(int nSocketIndex)
{
	LOGIN_LIST::iterator it = mLoginList.find(nSocketIndex);
	if (it != mLoginList.end())
	{
		// 完成使命，释放掉
		CObjPool::Inst()->free(it->second);
		mLoginList.erase(it);
	}
}

unsigned int CLoginJob::allocateRoleID(int nServerId)
{
	uint nMaxRoleId = getAllocateRoleId(nServerId);
	if (0 == nMaxRoleId)
	{
		return 0;
	}

	if (nMaxRoleId >= MAX_SERVER_ID * (uint)nServerId)
	{
		return 0;
	}

	setAllocateRoleId(nServerId, nMaxRoleId + 1);
	CDBModule::Inst()->pushDBTask(getJobID(), 0, emSessionType_UpdateAllocateRoleId, 0, 0,
		"update AllocateRoleId set max_role_id=%d where server_id=%d", nMaxRoleId + 1, nServerId);
	return nMaxRoleId;
}

/// 加载数据库里分配角色ID的字段
void CLoginJob::loadAllocateRoleId()
{
	//CDBModule::Inst()->pushDBTask(0, emSessionType_LoadAllocateRoleId, 0, 0, "select * from AllocateRoleId order by server_id asc limit %d", BAT_LOAD_ROLE_ID_NUM);
}

/// 加载数据库里分配角色ID的处理
void CLoginJob::onLoadAllocateRoleId(CDBResponse& rResponse)
{
	int nRow = rResponse.mRowNum;
	int nMaxServerId = 0;
	for (int i = 0; i < nRow; ++i)
	{
		int nServerID = rResponse.getInt();
		unsigned int nRoleID = rResponse.getInt();
		setAllocateRoleId(nServerID, nRoleID);
		nMaxServerId = nServerID;
	}

	// 还没加载完成
	if (nRow >= BAT_LOAD_ROLE_ID_NUM)
	{
		//CDBModule::Inst()->pushDBTask(0, emSessionType_LoadAllocateRoleId, 0, 0, "select * from AllocateRoleId where server_id > %d order by server_id asc limit %d", nMaxServerId, BAT_LOAD_ROLE_ID_NUM);
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
				LOG_ERROR("begin role id is zero, server id: %d", CGameServer::Inst()->getServerID());
			}
			else
			{
				setAllocateRoleId(CGameServer::Inst()->getServerID(), nBeginRoleID);
				//CDBModule::Inst()->pushDBTask(0, emSessionType_UpdateAllocateRoleId, 0, 0,
				//	"insert into AllocateRoleId (server_id,max_role_id) values(%d, %d)", CGameServer::Inst()->getServerID(), nBeginRoleID);
			}
		}
	}
}