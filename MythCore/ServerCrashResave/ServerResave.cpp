#include "ServerResave.h"
#include "servercommon.h"
#include "objpool.h"
#include "timemanager.h"
#include "gameserverconfig.h"
#include "logmanager.h"
#include "logdisplayer.h"
#include "log.h"
#include "internalmsgpool.h"
#include "jobmanager.h"
#include "scenejob.h"
#include "propertymodule.h"
#include "dbmodule.h"
#include "entityplayer.h"
#include "objpoolimp.h"
using namespace Myth;
CServerResave::CServerResave()
:mFiveSecTimer(5)
{
	
}

/// 初始化
bool CServerResave::initAll()
{
	// 优先初始化这两个变量
	bool bResult = initLogicModule();
	if (!bResult)
	{
		return false;
	}
	mpJobManager = CJobManager::Inst();
	mpJobManager->initMin();

	bResult = CGameServerConfig::Inst()->loadGameServerConfigFromXml("config/gameserverconfig.xml");
	if (!bResult)
	{
		return bResult;
	}

	CTimeManager::Inst()->setCurrTime(time(NULL));
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
	bResult = mpJobManager->initThread();
	if (!bResult)
	{
		return false;
	}

	bool bCreate = true;
	int nShareMemoryKey = 37345235;

	int nShareMemorySize = CObjPool::Inst()->getShareMemorySize();
	byte* pSharePoint = CShareMemory::createShareMemory(nShareMemoryKey, nShareMemorySize, bCreate);
	if (NULL == pSharePoint)
	{
		return false;
	}

	// 没有遗留的玩家
	if (bCreate)
	{
		CShareMemory::destroyShareMemory(nShareMemoryKey, pSharePoint);
		return false;
	}

	CObjPool::Inst()->setShareMemory(pSharePoint);
	ResetPlayerSaveStatus();
	return true;
}

/// 初始日志
bool CServerResave::initLog()
{
	CLogManager* pLogManger = CLogManager::createInst();
	if (NULL == pLogManger)
	{
		printf("create log manager failure");
		return false;
	}


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
bool CServerResave::initLogicModule()
{
	CTimeManager::createInst();
	CInternalMsgPool::createInst();
	CGameServerConfig::createInst();
	CObjPool::createInst();
	CJobManager::createInst();
	// 注意，不把CSceneJob放在job队列里面去管理
	CSceneJob::createInst();
	CPropertyModule::createInst();
	CDBModule::createInst();

	return true;
}


/// 初始化游戏静态数据
bool CServerResave::initStaticData()
{
	bool bResult = CGameServerConfig::Inst()->loadGameServerConfigFromXml("config/gameserverconfig.xml");
	if (!bResult)
	{
		return bResult;
	}
	return true;
}


/// 运行
void CServerResave::run()
{
	static int i = 0;
	while (true)
	{
		CTimeManager::Inst()->setCurrTime(time(NULL));
		time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
		if (tTimeNow != mLastTime)
		{
			// 就是秒
			if (mFiveSecTimer.elapse((int)(tTimeNow - mLastTime)))
			{
				SavePlayerBat();
			}
			mLastTime = tTimeNow;
		}

		//printf("*dddd*");
		mpJobManager->run();
#ifdef MYTH_OS_WINDOWS
		Sleep(20);
#else
		struct timespec tv;
		tv.tv_sec = 0;
		tv.tv_nsec = 20000000;
		nanosleep(&tv, NULL);
#endif
		// 如果是退出状态，并且所以的job都已经退出完成
		if (mExit && mpJobManager->checkAllJobExit())
		{
			break;
		}
	}
}

void CServerResave::ResetPlayerSaveStatus()
{
	int nMinID = CObjPool::Inst()->getPlayerMinID();
	int nMaxID = nMinID + CAPACITY_PLAYER;
	for (int i = nMinID; i < nMaxID; ++i)
	{
		CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(i);
		if (NULL == pPlayer)
		{
			continue;
		}

		if (INVALID_OBJ_ID == pPlayer->getObjID())
		{
			continue;
		}
		pPlayer->setSaveStatus(emSaveStatus_None);
	}
}

void CServerResave::SavePlayerBat()
{
	// 退出
	if (getExit())
	{
		return;
	}

	int nMinID = CObjPool::Inst()->getPlayerMinID();
	int nMaxID = nMinID + CAPACITY_PLAYER;

	int nCount = 0; 
	int nSavingCount = 0;

	time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
	for (int i = nMinID; i < nMaxID; ++i)
	{
		CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(i);
		if (NULL == pPlayer)
		{
			continue;
		}

		if (INVALID_OBJ_ID == pPlayer->getObjID())
		{
			continue;
		}
		// 已经在保存中了
		if (emSaveStatus_None != pPlayer->getSaveStatus())
		{
			// 没有保存完成
			if (emSaveStatusAll != pPlayer->getSaveStatus())
			{
				++ nSavingCount;
			}
			continue;
		}

		if (tTimeNow - pPlayer->getLastSaveTime() < 30)
		{
			continue;
		}

		pPlayer->setLastSaveTime(CTimeManager::Inst()->getCurrTime());
		CPropertyModule::Inst()->savePlayer(pPlayer);
		nCount++ ;
		if (nCount >= 50)
		{
			break;
		}
	}

	// 已经保存完成
	if (nCount <= 0 && nSavingCount <= 0)
	{
		printf("保存完成");
		setExit(true);
		CSceneJob::Inst()->setExited(true);
	}
}


/// 开始为退出做准备
void CServerResave::clear()
{
	if (NULL != mpJobManager)
	{
		mpJobManager->clearThread();
	}
	clearStaticData();
	clearLog();
	clearLogicModule();
}

/// 清理日志资源
void CServerResave::clearLog()
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
	for (int i = 0; i < nInfoDisplaySize; ++i)
	{
		delete CLogManager::Inst()->GetInfoLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetInfoLog().SetDisplayerSize(0);

	// 删除警告日志的displayer
	int nWarnDisplaySize = CLogManager::Inst()->GetWarnLog().GetDisplayerSize();
	for (int i = 0; i < nWarnDisplaySize; ++i)
	{
		delete CLogManager::Inst()->GetWarnLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetWarnLog().SetDisplayerSize(0);

	CLogManager::destroyInst();
}

/// 清理逻辑模块
void CServerResave::clearLogicModule()
{
	CObjPool::destroyInst();
	CGameServerConfig::destroyInst();
	CInternalMsgPool::destroyInst();
}

/// 清理静态数据
void CServerResave::clearStaticData()
{
}



/// 退出
void CServerResave::exit()
{
	clear();
}
