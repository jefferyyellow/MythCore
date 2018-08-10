#include "gameserver.h"
#include "logmanager.h"
#include "log.h"
#include "logdisplayer.h"
#include "loginmodule.h"
#include "internalmsgpool.h"
#include "gameserverconfig.h"
#include "objpool.h"
#include "timemanager.h"
#include "template.h"
#include "dirtyword.h"
CGameServer::CGameServer()
{
	init();
	mServerID = 1;
}

void CGameServer::init()
{
    mDefaultLog = NULL;
    mServerID = 0;
    mExit = false;
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

	CTimeManager::Inst()->setCurrTime(time(NULL));
	srand((unsigned int)CTimeManager::Inst()->getCurrTime());

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

	bResult = initThread();
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
	CRollFileDisplayer* pDefaultFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/gamedefault.log"), 1024000, 10);
	// 为默认的debug日志加文件displayer
	mDefaultLog->AddDisplayer(pDefaultFileDisplayer);
	CLogManager::Inst()->AddDebugLog(mDefaultLog, "default");
#endif

	// 错误日志加文件displayer
	CRollFileDisplayer* pErrorFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/gameerror.log"), 1024000, 10);
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pErrorFileDisplayer);

	// 错误日志加std displayer
	CStdDisplayer* pDisplayer = new CStdDisplayer();
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pDisplayer);
	
	// 给信息日志加文件displayer
	CRollFileDisplayer* pInfoFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/gameinfo.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pInfoFileDisplayer);

	// 给警告日志加文件displayer
	CRollFileDisplayer* pWarnFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/gamewarn.log"), 1024000, 10);
	CLogManager::Inst()->GetWarnLog().AddDisplayer(pWarnFileDisplayer);
	return true;
}


/// 初始逻辑模块
bool CGameServer::initLogicModule()
{
	CMessageFactory::createInst();
	CInternalMsgPool::createInst();
	CGameServerConfig::createInst();
	CObjPool::createInst();
	CTimeManager::createInst();
	CDirtyWord::createInst();
	return true;
}


/// 初始化游戏静态数据
bool CGameServer::initStaticData()
{
	bool bResult = CGameServerConfig::Inst()->loadGameServerConfigFromXml("config/gameserverconfig.xml");
	if (!bResult)
	{
		return bResult;
	}

	bResult = CStaticData::loadFromFile("gameserverconfig/template/template_server.dat");
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

/// 初始线程
bool CGameServer::initThread()
{
	mThreadPool.init(10);

	//Sleep(3000);
	mLocalLogJob.initAll();

	for (int i = 0; i < MAX_DB_JOB; ++ i)
	{
		if (0 != mDBJob[i].initDB(CGameServerConfig::Inst()->getDBHost(),
			CGameServerConfig::Inst()->getDBUserName(), CGameServerConfig::Inst()->getDBPasswd(),
			CGameServerConfig::Inst()->getDefaultDataBase(), CGameServerConfig::Inst()->getDBPort(),
			NULL))
		{
			return false;
		}
		mDBJob[i].setBuffer(MAX_DB_JOB_BUFFER_SIZE);
		mThreadPool.pushBackJob(&mDBJob[i]);
	}


	bool bResult = mPlatJob.initAll("192.168.10.13", 6379, 5, 8999);
	if (!bResult)
	{
		return false;
	}

	bResult = mSceneJob.initBase(MAX_SCENE_DB_BUFFER_SIZE);
	if (!bResult)
	{
		return false;
	}
	mSceneJob.doInit();

	printf("initThread\n");
	mThreadPool.pushBackJob(&mSceneJob);
	mThreadPool.pushBackJob(&mLocalLogJob);
	mThreadPool.pushBackJob(&mPlatJob);
	return true;
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
		CTimeManager::Inst()->setCurrTime(time(NULL));

		//printf("*dddd*");
		mThreadPool.run();
#ifdef MYTH_OS_WINDOWS
		Sleep(20);
#else
		struct timespec tv;
		tv.tv_sec = 0;
		tv.tv_nsec = 20000000;
		nanosleep(&tv, NULL);
#endif
		// 如果是退出状态，并且所以的job都已经退出完成
		if (mExit && checkAllJobExit())
		{
			break;
		}

		++ i;
		if ( i > 100)
		{
			mExit = true;
		}
	}
}


/// 开始为退出做准备
void CGameServer::clear()
{
	clearThread();
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

	CLogManager::destroyInst();
}

/// 清理逻辑模块
void CGameServer::clearLogicModule()
{
	CDirtyWord::destroyInst();
	CTimeManager::destroyInst();
	CObjPool::destroyInst();
	CGameServerConfig::destroyInst();
	CInternalMsgPool::destroyInst();
	CMessageFactory::destroyInst();
}

/// 清理静态数据
void CGameServer::clearStaticData()
{
	CStaticData::clearTemplate();
}

/// 清理线程
void CGameServer::clearThread()
{
	mThreadPool.terminateAllThread();
	mThreadPool.waitAllThread();
	mSceneJob.clearBase();
	mPlatJob.clear();

	for (int i = 0; i < MAX_DB_JOB; ++ i)
	{
		mDBJob[i].clear();
	}
}

/// 退出
void CGameServer::exit()
{
	clear();
}

/// 检查除了log job是否都已经退出
bool CGameServer::checkOtherJobExit()
{
	if (!mSceneJob.getExited())
	{
		return false;
	}

	if (!mPlatJob.getExited())
	{
		return false;
	}

	for (int i = 0; i < MAX_DB_JOB; ++ i)
	{
		if (!mDBJob[i].getExited())
		{
			return false;
		}
	}

	return true;
}

/// 检查所有的 job是否退出
bool CGameServer::checkAllJobExit()
{
	if (!mLocalLogJob.getExited())
	{
		return false;
	}

	return checkOtherJobExit();
}


void CGameServer::pushTask(EmJobTaskType eTaskType, CInternalMsg* pMsg)
{
	switch (eTaskType)
	{
		case emJobTaskType_LocalLog:
		{
			mLocalLogJob.pushTask(pMsg);
			break;
		}
		case emJobTaskType_Scene:
		{
			mSceneJob.pushTask(pMsg);
			break;
		}
		case emJobTaskType_Plat:
		{
			mPlatJob.pushTask(pMsg);
			break;
		}
		default:
		{
			break;
		}
	}
}

void CGameServer::pushDBTask(int nUid, byte* pData, int nDataLength)
{
	int nIndex = nUid % MAX_DB_JOB;
	mDBJob[nIndex].pushBackJobData(pData, nDataLength);
}
