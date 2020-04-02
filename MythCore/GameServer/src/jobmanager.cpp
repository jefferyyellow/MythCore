#include "jobmanager.h"
#include "locallogjob.h"
#include "dbjob.h"
#include "platjob.h"
#include "scenejob.h"
#include "gameserverconfig.h"
#include "internalmsg.h"

CJobManager::CJobManager()
{
	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		mpDBJob[i] = NULL;
	}
	mpLocalLogJob = NULL;
	mpSceneJob = NULL;
	mpPlatJob = NULL;
}

// 
bool CJobManager::init()
{
	mpLocalLogJob = new CLocalLogJob;
	if (NULL == mpLocalLogJob)
	{
		return false;
	}

	mpPlatJob = new CPlatJob;
	if (NULL == mpPlatJob)
	{
		return false;
	}

	// 初始化SceneJob
	mpSceneJob = CSceneJob::createInst();
	if (NULL == mpSceneJob)
	{
		return false;
	}

	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		mpDBJob[i] = new CDBJob;
		if (NULL == mpDBJob[i])
		{
			return false;
		}
	}

	return true;
}

/// 初始线程
bool CJobManager::initThread()
{
	mThreadPool.init(10);

	//Sleep(3000);
	// 初始化LocalLogJob

	mpLocalLogJob->initAll();

	// 初始化DBJob
	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		if (0 != mpDBJob[i]->initDB(CGameServerConfig::Inst()->getDBHost(),
			CGameServerConfig::Inst()->getDBUserName(), CGameServerConfig::Inst()->getDBPasswd(),
			CGameServerConfig::Inst()->getDefaultDataBase(), CGameServerConfig::Inst()->getDBPort(),
			NULL))
		{
			return false;
		}
		mpDBJob[i]->setBuffer(MAX_DB_JOB_BUFFER_SIZE);
	}

	if (NULL != mpPlatJob)
	{
		// 初始化PlatJob
		bool bResult = mpPlatJob->initAll("192.168.10.13", 6379, 5, 8999);
		if (!bResult)
		{
			return false;
		}
	}


	if (NULL != mpSceneJob)
	{
		bool bResult = mpSceneJob->initBase(MAX_SCENE_DB_BUFFER_SIZE);
		if (!bResult)
		{
			return false;
		}
		mpSceneJob->doInit();

	}


	printf("initThread\n");
	mThreadPool.pushBackJob(mpLocalLogJob);
	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		mThreadPool.pushBackJob(mpDBJob[i]);
	}
	if (NULL != mpSceneJob)
	{
		mThreadPool.pushBackJob(mpSceneJob);
	}
	if (NULL != mpPlatJob)
	{
		mThreadPool.pushBackJob(mpPlatJob);
	}
	return true;
}

/// 初始化最小化
bool CJobManager::initMin()
{
	mpLocalLogJob = new CLocalLogJob;
	if (NULL == mpLocalLogJob)
	{
		return false;
	}

	// 初始化DBJob
	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		mpDBJob[i] = new CDBJob;
		if (NULL == mpDBJob[i])
		{
			return false;
		}
	}
	return true;
}

/// 清理线程
void CJobManager::clearThread()
{
	mThreadPool.terminateAllThread();
	mThreadPool.waitAllThread();
	if (NULL != mpSceneJob)
	{
		mpSceneJob->clearBase();
	}
	if (NULL != mpPlatJob)
	{
		mpPlatJob->clear();
	}

	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		if (NULL == mpDBJob[i])
		{
			continue;
		}
		mpDBJob[i]->clear();
	}
}

void CJobManager::run()
{
	mThreadPool.run();
}


/// 检查除了log job是否都已经退出
bool CJobManager::checkOtherJobExit()
{
	if (NULL != mpSceneJob)
	{
		if (!mpSceneJob->getExited())
		{
			return false;
		}
	}

	if (NULL != mpPlatJob)
	{
		if (!mpPlatJob->getExited())
		{
			return false;
		}
	}


	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		if (NULL == mpDBJob[i])
		{
			continue;
		}
		if (!mpDBJob[i]->getExited())
		{
			return false;
		}
	}

	return true;
}

/// 检查所有的 job是否退出
bool CJobManager::checkAllJobExit()
{
	if (NULL != mpLocalLogJob)
	{
		if (!mpLocalLogJob->getExited())
		{
			return false;
		}
	}


	return checkOtherJobExit();
}

void CJobManager::pushTask(EmJobTaskType eTaskType, CInternalMsg* pMsg)
{
	switch (eTaskType)
	{
	case emJobTaskType_LocalLog:
	{
		mpLocalLogJob->pushTask(pMsg);
		break;
	}
	case emJobTaskType_Scene:
	{
		mpSceneJob->pushTask(pMsg);
		break;
	}
	case emJobTaskType_Plat:
	{
		mpPlatJob->pushTask(pMsg);
		break;
	}
	default:
	{
		break;
	}
	}
}

void CJobManager::pushDBTask(int nUid, byte* pData, int nDataLength)
{
	int nIndex = nUid % MAX_DB_JOB;
	mpDBJob[nIndex]->pushBackJobData(pData, nDataLength);
}
