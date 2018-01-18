#include "scenejob.h"
#include "messagefactory.h"
#include "loginmessage.hxx.pb.h"
#include "message.hxx.pb.h"
#include "loginmodule.h"
#include "internalmsgpool.h"
#include "entityplayer.h"
#include "propertymodule.h"
#include "itemmodule.h"
#include "gameserver.h"
#include "dbmodule.h"
#include "taskmodule.h"
#include "mapmodule.h"
#include "mapconfigmanager.h"
#include "mapmamager.h"

CSceneJob::CSceneJob()
	:mNewDayTimer(CHECK_NEW_DAY_INTERVAL)
{

}

void CSceneJob::doing(int uParam)
{
	//printf("CSceneJob::doing %d\n", uParam);
	switch (mServerState)
	{
		case emServerStateInit:
		{
			doInit();
			break;
		}
		case emServerStateLaunch:
		{
			doLaunch();
			break;
		}
		case emServerStateRun:
		{
			doRun();
			break;
		}
		case emServerStateExit:
		{
			doExit();
			break;
		}
		default:
		{
			break;
		}
	}



	//Sleep(3000);
}

void CSceneJob::doInit()
{
	launchServer();
	printf("Begin Launch Server\n");
	mServerState = emServerStateLaunch;
}

void CSceneJob::doLaunch()
{
	checkDBStream();
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}
		onTask(pIMMsg);

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
	}
	if (checkLaunch())
	{
		printf("Server Launch Complete\n");
		launchComplete();
		mServerState = emServerStateRun;
	}
}

void CSceneJob::doRun()
{
	processClientMessage();
	checkDBStream();
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}
		onTask(pIMMsg);

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
	}
	int nElapseTime = (int)(getTickCount() - mLastTimerTick);
	if (nElapseTime > 100)
	{
		timer(nElapseTime);
		mLastTimerTick = getTickCount();
	}
	uint64 tTimeNow = CTimeManager::Inst()->getCurrTime();
	if (tTimeNow != mLastTime)
	{
		setTmNow(tTimeNow);
		mLastTime = tTimeNow;
	}
}

void CSceneJob::doExit()
{
	exitServer();
	checkDBStream();
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}
		onTask(pIMMsg);

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
	}
	if (0 == mPlayerList.size())
	{
		printf("All Player Kick Out\n");
	}
}

void CSceneJob::launchServer()
{
	LOGIC_MODULE_LIST::iterator it = mLogicModuleList.begin();
	for (; it != mLogicModuleList.end(); ++ it)
	{
		(*it)->onLaunchServer();
	}
}

bool CSceneJob::checkLaunch()
{
	bool bResult = false;
	LOGIC_MODULE_LIST::iterator it = mLogicModuleList.begin();
	for (; it != mLogicModuleList.end(); ++it)
	{
		bResult = (*it)->onCheckLaunch();
		if (!bResult)
		{
			return bResult;
		}
	}

	return true;
}

void CSceneJob::launchComplete()
{
	LOGIC_MODULE_LIST::iterator it = mLogicModuleList.begin();
	for (; it != mLogicModuleList.end(); ++it)
	{
		(*it)->onLaunchComplete();
	}
}

void CSceneJob::exitServer()
{
	LOGIC_MODULE_LIST::iterator it = mLogicModuleList.begin();
	for (; it != mLogicModuleList.end(); ++it)
	{
		(*it)->onExitServer();
	}
}

void CSceneJob::newDayCome()
{
	LOGIC_MODULE_LIST::iterator it = mLogicModuleList.begin();
	for (; it != mLogicModuleList.end(); ++it)
	{
		(*it)->onNewDayCome();
	}
}

void CSceneJob::newWeekCome()
{
	LOGIC_MODULE_LIST::iterator it = mLogicModuleList.begin();
	for (; it != mLogicModuleList.end(); ++it)
	{
		(*it)->onNewWeekCome();
	}
}

void CSceneJob::createPlayer(CEntityPlayer* pPlayer)
{
	LOGIC_MODULE_LIST::iterator it = mLogicModuleList.begin();
	for (; it != mLogicModuleList.end(); ++it)
	{
		(*it)->onCreatePlayer(pPlayer);
	}
}

void CSceneJob::destroyPlayer(CEntityPlayer* pPlayer)
{
	LOGIC_MODULE_LIST::iterator it = mLogicModuleList.begin();
	for (; it != mLogicModuleList.end(); ++it)
	{
		(*it)->onDestroyPlayer(pPlayer);
	}
}

/// 时间函数
void CSceneJob::timer(unsigned int nTickOffset)
{
	LOGIC_MODULE_LIST::iterator it = mLogicModuleList.begin();
	for (; it != mLogicModuleList.end(); ++it)
	{
		(*it)->onTimer(nTickOffset);
	}

	if (mNewDayTimer.elapse(nTickOffset))
	{
		checkNewDayCome();
	}
}

void CSceneJob::checkNewDayCome()
{
	struct tm& tmNow = getTmNow();

	if (0 == tmNow.tm_hour && 0 == tmNow.tm_min)
	{
		newDayCome();
		// 0表示星期天，1表示星期一
		if (1 == tmNow.tm_wday)
		{
			newWeekCome();
		}
		// 300秒以后在检查，然后恢复30秒检查一次
		mNewDayTimer.setLeftTime(CHECK_NEW_DAY_INTERVAL * 10);
	}
}

void CSceneJob::onTask(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	//switch (pMsg->getMsgID())
	//{
	//	default:
	//		break;
	//}
}

bool CSceneJob::init(int nDBBuffSize)
{
	// 初始化时间变量
	mLastTimerTick = getTickCount();
	mLastTime = CTimeManager::Inst()->getCurrTime();
	mServerState = emServerStateInit;
	bool bResult = initShareMemory();
	if (!bResult)
	{
		return false;
	}
	
	mDBBuffer = new byte[nDBBuffSize];
	mDBStream.Initialize(mDBBuffer, nDBBuffSize);

	// 逻辑模块
	CMapConfigManager::CreateInst();
	CMapManager::CreateInst();

	mLogicModuleList.push_back(CLoginModule::CreateInst());
	mLogicModuleList.push_back(CPropertyModule::CreateInst());
	mLogicModuleList.push_back(CMapModule::CreateInst());
	mLogicModuleList.push_back(CDBModule::CreateInst());

	return true;
}

/// 初始化管道
bool CSceneJob::initShareMemory()
{
	// 一个CShareMemory管理类
	int nShareMemorySize = sizeof(CShareMemory);
	// 两个CSocketStream管理类
	nShareMemorySize += 2 * sizeof(CSocketStream);
	// 两个真实的内存区域
	nShareMemorySize += 2 * PIPE_SIZE;

	bool bCreate = true;
	byte* pSharePoint = CShareMemory::createShareMemory(37345234, nShareMemorySize, bCreate);
	if (NULL == pSharePoint)
	{
		return false;
	}
	// 初始化
	mShareMemory = (CShareMemory*)pSharePoint;
	if (bCreate)
	{
		mShareMemory->setShmPoint(pSharePoint);
		mShareMemory->setShmKey(37345234);
		mShareMemory->setShmSize(nShareMemorySize);
	}


	pSharePoint += sizeof(CShareMemory);

	// 初始化Tcp2Server共享内存
	mTcp2ServerMemory = (CSocketStream*)pSharePoint;
	if (bCreate)
	{
		mTcp2ServerMemory->Initialize(pSharePoint + sizeof(CSocketStream), PIPE_SIZE);
	}

	pSharePoint += sizeof(CSocketStream) + PIPE_SIZE;
	// 初始化Server2Tcp共享内存
	mServer2TcpMemory = (CSocketStream*)pSharePoint;
	if (bCreate)
	{
		mServer2TcpMemory->Initialize(pSharePoint + sizeof(CSocketStream), PIPE_SIZE);
	}
	return true;
}

/// 压入DB数据
void CSceneJob::pushBackDBData(byte* pData, int nDataLength)
{
	if (NULL == pData || 0 == nDataLength)
	{
		return;
	}
	mDBStreamLock.lock();
	int nResult = mDBStream.PushPacket(pData, nDataLength);
	mDBStreamLock.unlock();
}

/// 取出DB数据
void CSceneJob::popUpDBData(byte* pData, int &rLength)
{
	if (NULL == pData)
	{
		return;
	}
	mDBStreamLock.lock();
	int nResult = mDBStream.GetHeadPacket(pData, rLength);
	mDBStreamLock.unlock();
}

/// 检查DB流
void CSceneJob::checkDBStream()
{
	for (int i = 0; i < 2000; ++ i)
	{
		int nLength = 0;
		popUpDBData((byte*)CDBModule::Inst()->getDBResponse(), nLength);
		if (nLength <= 0)
		{
			return;
		}

		CDBModule::Inst()->onDBSession();
	}
}


/// 处理前端消息
void CSceneJob::processClientMessage()
{
	int nMessageLen = MAX_SOCKET_BUFF_SIZE;
	int nResult = 0;
	for (int i = 0; i < 2000; ++i)
	{
		nResult = mTcp2ServerMemory->GetHeadPacket((byte*)mBuffer, nMessageLen);
		if (nResult < 0)
		{
			break;
		}

		if (nMessageLen <= 0)
		{
			break;
		}

		char* pTemp = mBuffer;
		CExchangeHead* pExchangeHead = (CExchangeHead*)mBuffer;

		pTemp += sizeof(CExchangeHead);
		nMessageLen -= sizeof(CExchangeHead);
		if (nMessageLen <= 0)
		{
			if (pExchangeHead->mSocketError)
			{
				onSocketDisconnect(pExchangeHead->mSocketIndex);
			}
			continue;
		}

		short nLength = *(short*)pTemp;
		if (nLength != nMessageLen)
		{
			break;
		}

		pTemp += sizeof(short);
		nMessageLen -= sizeof(short);

		short nMessageID = *(short*)pTemp;
		pTemp += sizeof(short);
		nMessageLen -= sizeof(short);

		Message* pMessage = CMessageFactory::Inst()->createClientMessage(nMessageID);
		if (NULL != pMessage)
		{
			printf("Recive MessageID: %d\n", nMessageID);
			pMessage->ParseFromArray(pTemp, nMessageLen);
			int nModule = nMessageID & MESSAGE_MODULE_MASK;
			if (nModule == MESSAGE_MODULE_LOGIN)
			{
				// 如果已经在场景中了，发登录的消息没有意义
				PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(pExchangeHead->mSocketIndex);
				if (it != mPlayerSocketList.end())
				{
					return;
				}
				CLoginModule::Inst()->onClientMessage(*pExchangeHead, nMessageID, pMessage);
			}
			else
			{
				// 还没有登录玩家，发其他的消息没有意义
				PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(pExchangeHead->mSocketIndex);
				if (it == mPlayerSocketList.end())
				{
					return;
				}

				CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
				if (NULL == pPlayer)
				{
					return;
				}

				// 只有在游戏状态下才接受正常的消息，避免数据的不一致
				if (emPlayerStatus_Gameing != pPlayer->getPlayerStauts())
				{
					return;
				}

				dispatchClientMessage(pPlayer, nMessageID, pMessage);
			}
		}
	}
}

/// 发送前端消息
void CSceneJob::send2Player(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage)
{
	char* pTemp = mBuffer;
	memcpy(pTemp, &rExchangeHead, sizeof(rExchangeHead));
	pTemp += sizeof(rExchangeHead);

	unsigned short nMessageLen = pMessage->ByteSize() + sizeof(unsigned short) * 2;
	memcpy(pTemp, &nMessageLen, sizeof(nMessageLen));

	pTemp += sizeof(nMessageLen);

	memcpy(pTemp, &nMessageID, sizeof(nMessageID));
	pTemp += sizeof(nMessageID);

	pMessage->SerializeToArray(pTemp, sizeof(mBuffer) - sizeof(rExchangeHead) - sizeof(unsigned short) * 2);
	printf("PushPacket nMessageID: %d\n", nMessageID);
	mServer2TcpMemory->PushPacket((byte*)mBuffer, pMessage->ByteSize() + sizeof(rExchangeHead) + sizeof(unsigned short) * 2);

}

void CSceneJob::send2Player(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	send2Player(pPlayer->GetExhangeHead(), nMessageID, pMessage);
}

/// 发生给所有的玩家消息
void CSceneJob::send2AllPlayer(unsigned short nMessageID, Message* pMessage)
{
	CEntityPlayer* pPlayer = NULL;
	PLAYER_LIST::iterator it = mPlayerList.begin();
	for (; it != mPlayerList.end(); ++ it)
	{
		pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
		if (NULL == pPlayer)
		{
			continue;
		}
		send2Player(pPlayer->GetExhangeHead(), nMessageID, pMessage);
	}
}


/// 断开玩家的连接
void CSceneJob::disconnectPlayer(CExchangeHead& rExchangeHead)
{
	char* pTemp = mBuffer;
	rExchangeHead.mSocketError = emTcpError_OffLineClose;
	memcpy(pTemp, &rExchangeHead, sizeof(rExchangeHead));
	mServer2TcpMemory->PushPacket((byte*)mBuffer, sizeof(rExchangeHead));
}

void CSceneJob::disconnectPlayer(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}
	// 端口连接的时候，将玩家的socket信息清除
	removePlayerSocketIndex(pPlayer->GetExhangeHead().mSocketIndex);
	disconnectPlayer(pPlayer->GetExhangeHead());
}

/// 分发前端消息
void CSceneJob::dispatchClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage)
{
	int nModule = nMessageID & MESSAGE_MODULE_MASK;
	switch (nModule)
	{
		case MESSAGE_MODULE_PROPERTY:
		{
			CPropertyModule::Inst()->onClientMessage(pPlayer, nMessageID, pMessage);
			break;
		}
		case MESSAGE_MODULE_ITEM:
		{
			CItemModule::Inst()->onClientMessage(pPlayer, nMessageID, pMessage);
			break;
		}
		case MESSAGE_MODULE_MAP:
		{
			CMapModule::Inst()->onClientMessage(pPlayer, nMessageID, pMessage);
			break;
		}
		case MESSAGE_MODULE_TASK:
		{
			CTaskModule::Inst()->onClientMessage(pPlayer, nMessageID, pMessage);
			break;
		}
		default:
			break;
	}
}

/// 登录了一个玩家（只是登录校验完成，数据还没有加载完成）
bool CSceneJob::onPlayerLogin(CEntityPlayer* pNewPlayer)
{
	std::pair<PLAYER_SOCKET_LIST::iterator, bool> tSocketIndexRet = mPlayerSocketList.insert(
		PLAYER_SOCKET_LIST::value_type(pNewPlayer->GetExhangeHead().mSocketIndex, pNewPlayer->getObjID()));
		if (!tSocketIndexRet.second)
		{
			return false;
		}

	std::pair<PLAYER_LIST::iterator, bool> tPlayerListRet = mPlayerList.insert(
		PLAYER_LIST::value_type(pNewPlayer->getRoleID(), pNewPlayer->getObjID()));
		if (!tPlayerListRet.second)
		{
			mPlayerSocketList.erase(tSocketIndexRet.first);
			return false;
		}

	return true;
}

/// 离开了一个玩家
void CSceneJob::onPlayerLeaveGame(CEntityPlayer* pPlayer)
{
	mPlayerSocketList.erase(pPlayer->GetExhangeHead().mSocketIndex);
	mPlayerList.erase(pPlayer->getRoleID());
}

/// 一个Socket断开
void CSceneJob::onSocketDisconnect(int nSocketIndex)
{
	printf("Socket disconnect\n");
	CLoginModule::Inst()->onSocketDisconnect(nSocketIndex);
	PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(nSocketIndex);
	if (it != mPlayerSocketList.end())
	{
		CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
		if (NULL != pPlayer)
		{
			// 将玩家置为下线状态
			pPlayer->setPlayerStauts(emPlayerStatus_Exiting);
			CPropertyModule::Inst()->savePlayer(pPlayer);
		}
		else
		{
			mPlayerSocketList.erase(it);
		}
	}
}



/// 通过角色ID得到玩家
CEntityPlayer* CSceneJob::getPlayerByRoleID(unsigned int nRoleID)
{
	PLAYER_LIST::iterator it = mPlayerList.find(nRoleID);
	if (it != mPlayerList.end())
	{
		return static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
	}
	return NULL;
}

/// 通过SocketIndex得到玩家
CEntityPlayer* CSceneJob::getPlayerBySocketIndex(short nSocketIndex)
{
	PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(nSocketIndex);
	if (it != mPlayerSocketList.end())
	{
		return static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
	}

	return NULL;
}

/// 添加socket index
bool CSceneJob::addPlayerSocketIndex(short nSocketIndex, int nObjID)
{
	std::pair<PLAYER_SOCKET_LIST::iterator, bool> tSocketIndexRet = mPlayerSocketList.insert(
	PLAYER_SOCKET_LIST::value_type(nSocketIndex, nObjID));
	if (tSocketIndexRet.second)
	{
		return true;
	}
	return false;
}

/// 删除socket index
void CSceneJob::removePlayerSocketIndex(short nSocketIndex)
{
	mPlayerSocketList.erase(nSocketIndex);
}