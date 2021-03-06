#include "gameserver.h"
#include "logmanager.h"
#include "log.h"
#include "logdisplayer.h"
#include "loginmodule.h"
#include "internalmsgpool.h"
#include "gameserverconfig.h"
#include "objpool.h"
#include "template.h"
#include "dirtyword.h"
#include "perfmanager.h"
#include "locallogjob.h"
#include "jobmanager.h"
#include "GameLogicConfig.h"
#include "timemanager.h"
CGameServer::CGameServer()
	:mMinuteTimer(SECONDS_PER_MIN)
{
	init();
	mServerID = 1;
}

void CGameServer::init()
{
    mDefaultLog = NULL;
    mServerID = 0;
    mExit = false;
    mLastTime = 0;
	mpJobManager = NULL;
	mGameTimeOffset = 0;
}

/// 初始化
bool CGameServer::initAll()
{
	// 优先初始化这两个变量
	bool bResult = initLogicModule();
	if (!bResult)
	{
		return false;
	}

	mpJobManager = CJobManager::Inst();
	mpJobManager->init();

	bResult = CGameServerConfig::Inst()->loadGameServerConfigFromXml("config/gameserverconfig.xml");
	if (!bResult)
	{
		return bResult;
	}
	bResult = CGameLogicConfig::Inst()->loadGameLogicConfig("gameserverconfig/gamelogicconfig/gamelogicconfig.xml");
	if (!bResult)
	{
		return bResult;
	}

	CTimeManager::Inst()->setCurrTime(time(NULL) + mGameTimeOffset);
	srand((unsigned int)CTimeManager::Inst()->getCurrTime());
	mLastTime = CTimeManager::Inst()->getCurrTime();
	
	bResult = initLog();
	if (!bResult)
	{
		return false;
	}

	bResult = initStaticData();
	if (!bResult)
	{
		return false;
	}

	bResult = initNet();
	if (!bResult)
	{
		return false;
	}

	bResult = mpJobManager->initThread();
	if (!bResult)
	{
		return false;
	}
	return true;
}

/// 初始日志
bool CGameServer::initLog()
{
	CLogManager* pLogManger = CLogManager::createInst();
	if (NULL == pLogManger)
	{
		printf("create log manager failure");
		return false;
	}

#ifdef __DEBUG__
	// 默认日志
	mDefaultLog = new CLog();
	if (NULL == mDefaultLog)
	{
		printf("create default log failure");
		return false;
	}

	// 默认的debug日志
	CRollFileDisplayer* pDefaultFileDisplayer = new CRollFileDisplayer(const_cast<char*>("log/gamedefault.log"), 1024000, 10);
	// 为默认的debug日志加文件displayer
	mDefaultLog->AddDisplayer(pDefaultFileDisplayer);
	CLogManager::Inst()->AddDebugLog(mDefaultLog, "default");
#endif

	// 错误日志加文件displayer
	CRollFileDisplayer* pErrorFileDisplayer = new CRollFileDisplayer(const_cast<char*>("log/gameerror.log"), 1024000, 10);
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pErrorFileDisplayer);

	// 错误日志加std displayer
	CStdDisplayer* pDisplayer = new CStdDisplayer();
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pDisplayer);
	
	// 给信息日志加文件displayer
	CRollFileDisplayer* pInfoFileDisplayer = new CRollFileDisplayer(const_cast<char*>("log/gameinfo.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pInfoFileDisplayer);

	// 给警告日志加文件displayer
	CRollFileDisplayer* pWarnFileDisplayer = new CRollFileDisplayer(const_cast<char*>("log/gamewarn.log"), 1024000, 10);
	CLogManager::Inst()->GetWarnLog().AddDisplayer(pWarnFileDisplayer);
	return true;
}


/// 初始逻辑模块
bool CGameServer::initLogicModule()
{
	CPerfManager::createInst();
	CMessageFactory::createInst();
	//CInternalMsgPool::createInst();
	CGameServerConfig::createInst();
	CGameLogicConfig::createInst();
	CObjPool::createInst();
	CTimeManager::createInst();
	CDirtyWord::createInst();
	CJobManager::createInst();
	return true;
}


/// 初始化游戏静态数据
bool CGameServer::initStaticData()
{
	bool bResult = CStaticData::loadFromFile("gameserverconfig/template/template_server.dat");
	if (!bResult)
	{
		return bResult;
	}

	bResult = CDirtyWord::Inst()->loadDirtyWordConfig("gameserverconfig/dirtyword/dirtyword.dat");
	if (!bResult)
	{
		return bResult;
	}
	return true;
}


/// 初始化服务器网络
bool CGameServer::initNet()
{
	int nListenPort = CGameServerConfig::Inst()->getListenPort();
	bool bResult = mGameServerNet.initSocket(1, SOCKET_CACHE_SIZE, &nListenPort, 1, 1024);
	return bResult;
}

/// 运行
void CGameServer::run()
{
	//LOG_ERROR("Hello World");
	//MYTH_ASSERT(1,;);
	//MYTH_ASSERT_INFO(1,;,"I love you macro!");
	static int i = 0;
	while (true)
	{
		CTimeManager::Inst()->setCurrTime(time(NULL) + mGameTimeOffset);

		//printf("*dddd*");
		mGameServerNet.receiveMessage();
		mpJobManager->run();
#ifdef MYTH_OS_WINDOWS
		Sleep(20);
#else
		struct timespec tv;
		tv.tv_sec = 0;
		tv.tv_nsec = 20000000;
		nanosleep(&tv, NULL);
#endif
		time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
		if (tTimeNow != mLastTime)
		{
			// 就是秒
			if (mMinuteTimer.elapse((int)(tTimeNow - mLastTime)))
			{
				logPerf();
			}
			mLastTime = tTimeNow;
		}

		// 如果是退出状态，并且所以的job都已经退出完成
		if (mExit && mpJobManager->checkAllJobExit())
		{
			break;
		}
	}
}


/// 开始为退出做准备
void CGameServer::clear()
{
	mpJobManager->clearThread();
	clearStaticData();
	clearLog();
	clearLogicModule();
}

/// 清理日志资源
void CGameServer::clearLog()
{
	// 删除错误日志的displayer
	byte nErrorDisplaySize = CLogManager::Inst()->GetErrorLog().GetDisplayerSize();
	for (int i = 0; i < nErrorDisplaySize; ++i)
	{
		delete CLogManager::Inst()->GetErrorLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetErrorLog().SetDisplayerSize(0);

	// 删除信息日志的displayer
	byte nInfoDisplaySize = CLogManager::Inst()->GetInfoLog().GetDisplayerSize();
	for (int i = 0; i < nInfoDisplaySize; ++ i)
	{
		delete CLogManager::Inst()->GetInfoLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetInfoLog().SetDisplayerSize(0);

	// 删除警告日志的displayer
	int nWarnDisplaySize = CLogManager::Inst()->GetWarnLog().GetDisplayerSize();
	for (int i = 0; i < nWarnDisplaySize; ++ i)
	{
		delete CLogManager::Inst()->GetWarnLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetWarnLog().SetDisplayerSize(0);

#ifdef __DEBUG__
	// 删除默认日志的displayer
	if (NULL != mDefaultLog)
	{
		int nDefaultDisplaySize = mDefaultLog->GetDisplayerSize();
		for (int i = 0; i < nDefaultDisplaySize; ++ i)
		{
			delete mDefaultLog->GetDisplayer(i);
		}

		delete mDefaultLog;
	}
#endif

	CLogManager::destroyInst();
}

/// 清理逻辑模块
void CGameServer::clearLogicModule()
{
	CDirtyWord::destroyInst();
	CTimeManager::destroyInst();
	CObjPool::destroyInst();
	CGameLogicConfig::destroyInst();
	CGameServerConfig::destroyInst();
	//CInternalMsgPool::destroyInst();
	CMessageFactory::destroyInst();
	CPerfManager::destroyInst();
}

/// 清理静态数据
void CGameServer::clearStaticData()
{
	CStaticData::clearTemplate();
}



/// 退出
void CGameServer::exit()
{
	clear();
}


/// 记录所有的Perf的记录
void CGameServer::logPerf()
{
	CSimpleLock& rPerfLock = CPerfManager::Inst()->getLock();
	rPerfLock.lock();
	CPerfManager::PERF_HASH& rHashMap = CPerfManager::Inst()->getPerfHash();
	CPerfManager::PERF_HASH::iterator it = rHashMap.begin();
	char tBuffer[1024];
	int nOffset = 0;
	nOffset = snprintf(tBuffer, sizeof(tBuffer) - 1 - nOffset, "%40s\t%8s\t%8s\t%8s\t%8s\t%8s\n",
		"name", "call", "cost(us)", "most(us)", "lest(us)", "avg(us)");

	int nCount = 0;
	for (; it != rHashMap.end(); ++ it)
	{
		CPerfData& rData = it->second;
		nOffset += snprintf(tBuffer + nOffset, sizeof(tBuffer) - nOffset - 1, "%40s\t%8d\t%8d\t%8d\t%8d\t%8d\n", 
			it->first.c_str(), rData.getCallTimes(), rData.getCostTime(), rData.getMaxTime(), rData.getMinTime(), 
			(rData.getCallTimes() != 0) ? rData.getCostTime() / rData.getCallTimes() : 0);
		++ nCount;
		if (nCount >= 10)
		{
			LOG_INFO(tBuffer);
			nCount = 0;
			nOffset = 0;
		}
	}
	if (nCount > 0)
	{
		LOG_INFO(tBuffer);
	}
	rPerfLock.unlock();
}


///// 断开玩家的连接
//void CSceneJob::disconnectPlayer(CExchangeHead& rExchangeHead)
//{
//	char* pTemp = mBuffer;
//	rExchangeHead.mSocketError = emTcpError_OffLineClose;
//	memcpy(pTemp, &rExchangeHead, sizeof(rExchangeHead));
//	mServer2TcpMemory->PushPacket((byte*)mBuffer, sizeof(rExchangeHead));
//}
//
//void CSceneJob::disconnectPlayer(CEntityPlayer& rPlayer)
//{
//	// 端口连接的时候，将玩家的socket信息清除
//	removePlayerSocketIndex(rPlayer.getExchangeHead().mSocketIndex);
//	disconnectPlayer(rPlayer.getExchangeHead());
//}
//
///// 登录了一个玩家（只是登录校验完成，数据还没有加载完成）
//bool CSceneJob::onPlayerLogin(CEntityPlayer* pNewPlayer)
//{
//	std::pair<PLAYER_SOCKET_LIST::iterator, bool> tSocketIndexRet = mPlayerSocketList.insert(
//		PLAYER_SOCKET_LIST::value_type(pNewPlayer->getExchangeHead().mSocketIndex, pNewPlayer->getObjID()));
//	if (!tSocketIndexRet.second)
//	{
//		return false;
//	}
//
//	std::pair<PLAYER_LIST::iterator, bool> tPlayerListRet = mPlayerList.insert(
//		PLAYER_LIST::value_type(pNewPlayer->getRoleID(), pNewPlayer->getObjID()));
//	if (!tPlayerListRet.second)
//	{
//		mPlayerSocketList.erase(tSocketIndexRet.first);
//		return false;
//	}
//
//	return true;
//}
//
///// 离开了一个玩家
//void CSceneJob::destroyPlayerObject(CEntityPlayer& rPlayer)
//{
//	mPlayerList.erase(rPlayer.getRoleID());
//	disconnectPlayer(rPlayer);
//}
//
///// 一个Socket断开
//void CSceneJob::onSocketDisconnect(int nSocketIndex)
//{
//	printf("Socket disconnect\n");
//	CLoginModule::Inst()->onSocketDisconnect(nSocketIndex);
//	PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(nSocketIndex);
//	if (it != mPlayerSocketList.end())
//	{
//		// 注意只有玩家已经在游戏当中了才存盘，如果玩家是退出状态或者加载折腾，不需要存盘
//		CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
//		if (NULL != pPlayer)
//		{
//			pPlayer->getExchangeHead().mSocketIndex = -1;
//			// 将玩家置为下线状态
//			CPropertyModule::Inst()->playerLeaveGame(*pPlayer, EmLeaveReason_Disconnection);
//		}
//		mPlayerSocketList.erase(it);
//	}
//}

//
///// 通过角色ID得到玩家
//CEntityPlayer* CSceneJob::getPlayerByRoleID(unsigned int nRoleID)
//{
//	PLAYER_LIST::iterator it = mPlayerList.find(nRoleID);
//	if (it != mPlayerList.end())
//	{
//		return static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
//	}
//	return NULL;
//}
//
///// 通过SocketIndex得到玩家
//CEntityPlayer* CSceneJob::getPlayerBySocketIndex(short nSocketIndex)
//{
//	PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(nSocketIndex);
//	if (it != mPlayerSocketList.end())
//	{
//		return static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
//	}
//
//	return NULL;
//}
//
///// 添加socket index
//bool CSceneJob::addPlayerSocketIndex(short nSocketIndex, int nObjID)
//{
//	std::pair<PLAYER_SOCKET_LIST::iterator, bool> tSocketIndexRet = mPlayerSocketList.insert(
//		PLAYER_SOCKET_LIST::value_type(nSocketIndex, nObjID));
//	if (tSocketIndexRet.second)
//	{
//		return true;
//	}
//	return false;
//}
//
///// 删除socket index
//void CSceneJob::removePlayerSocketIndex(short nSocketIndex)
//{
//	mPlayerSocketList.erase(nSocketIndex);
//}