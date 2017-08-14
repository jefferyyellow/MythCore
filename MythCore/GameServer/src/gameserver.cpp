#include "gameserver.h"
#include "logmanager.h"
#include "log.h"
#include "logdisplayer.h"
#include "loginmodule.h"
#include "internalmsgpool.h"
#include "gameserverconfig.h"
#include "objpool.h"

/// 初始化
bool CGameServer::init()
{
	bool bResult = initLog();
	if (!bResult)
	{
		return false;
	}

	bResult = initLogicModule();
	if (!bResult)
	{
		return false;
	}

	CGameServerConfig::Inst()->loadGameServerConfigFromXml("config/gameserverconfig.xml");

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
	CLogManager* pLogManger = CLogManager::CreateInst();
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
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pWarnFileDisplayer);
	return true;
}


/// 初始逻辑模块
bool CGameServer::initLogicModule()
{
	CLoginModule::CreateInst();
	CMessageFactory::CreateInst();
	CInternalMsgPool::CreateInst();
	CGameServerConfig::CreateInst();
	CObjPool::CreateInst();
	return true;
}

/// 初始线程
bool CGameServer::initThread()
{
	mThreadPool = new Myth::CThreadPool(3);
	if (NULL == mThreadPool)
	{
		return false;
	}
	
	if (0 != mDBJob.init(CGameServerConfig::Inst()->getDBHost(), 
		CGameServerConfig::Inst()->getDBUserName(), CGameServerConfig::Inst()->getDBPasswd(),
		CGameServerConfig::Inst()->getDefaultDataBase(), CGameServerConfig::Inst()->getDBPort(),
		NULL))
	{
		return false;
	}

	bool bResult = mSceneJob.init();
	if (!bResult)
	{
		return false;
	}

	mThreadPool->pushBackJob(&mSceneJob);
	mThreadPool->pushBackJob(&mDBJob);
	mThreadPool->pushBackJob(&mLocalLogJob);

	return true;
}

/// 运行
void CGameServer::run()
{
	LOG_ERROR("Hello World");
	while (true)
	{
		mThreadPool->run();
#ifdef MYTH_OS_WINDOWS
		Sleep(5);
#else
		struct timespec tv;
		tv.tv_sec = 0;
		tv.tv_nsec = 5000;
		nanosleep(&tv, NULL);
#endif
	}
}


/// 开始为退出做准备
void CGameServer::clear()
{

}

/// 清理日志资源
void CGameServer::clearLog()
{
	// 删除错误日志的displayer
	uint8 nErrorDisplaySize = CLogManager::Inst()->GetErrorLog().GetDisplayerSize();
	for (int i = 0; i < nErrorDisplaySize; ++i)
	{
		delete CLogManager::Inst()->GetErrorLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetErrorLog().SetDisplayerSize(0);

	// 删除信息日志的displayer
	uint8 nInfoDisplaySize = CLogManager::Inst()->GetInfoLog().GetDisplayerSize();
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
}

/// 退出
void CGameServer::exit()
{

}

void CGameServer::pushTask(EmTaskType eTaskType, CInternalMsg* pMsg)
{
	switch (eTaskType)
	{
		case emTaskType_DB:
		{
			mDBJob.pushTask(pMsg);
			break;
		}
		case emTaskType_LocalLog:
		{
			mLocalLogJob.pushTask(pMsg);
			break;
		}
		case emTaskType_Scene:
		{
			mSceneJob.pushTask(pMsg);
			break;
		}
		default:
		{
			break;
		}
	}
}