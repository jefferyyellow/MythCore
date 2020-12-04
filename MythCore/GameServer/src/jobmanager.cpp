#include "jobmanager.h"
#include "locallogjob.h"
#include "dbjob.h"
#include "platjob.h"
#include "mapjob.h"
#include "gameserverconfig.h"
#include "internalmsg.h"
#include "rankjob.h"
#include "loginjob.h"

CJobManager::CJobManager()
{
	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		mpDBJob[i] = NULL;
	}
	for (int i = 0; i < MAX_MAP_JOB; ++ i)
	{
		mpMapJob[i] = NULL;
	}
	mpLocalLogJob = NULL;
	mpPlatJob = NULL;
	mpRankJob = NULL;
	mpLoginJob = NULL;
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

	mpRankJob = new CRankJob;
	if (NULL == mpRankJob)
	{
		return false;
	}

	mpLoginJob = new CLoginJob;
	if (NULL == mpLoginJob)
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

	for (int i = 0; i < MAX_MAP_JOB; ++ i)
	{
		mpMapJob[i] = new CMapJob;
		if (NULL == mpMapJob[i])
		{
			return false;
		}
	}

	return true;
}

/// 初始线程
bool CJobManager::initThread()
{
	mThreadPool.initThread(10);

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



	printf("initThread\n");
	pushBackJob(mpLocalLogJob);
	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		pushBackJob(mpDBJob[i]);
	}
	for (int i = 0; i < MAX_MAP_JOB; ++ i)
	{
		pushBackJob(mpMapJob[i]);
	}
	if (NULL != mpPlatJob)
	{
		pushBackJob(mpPlatJob);
	}
	if (NULL != mpRankJob)
	{
		pushBackJob(mpRankJob);
	}
	if (NULL != mpLoginJob)
	{
		pushBackJob(mpLoginJob);
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
	mThreadPool.run();
	mThreadPool.waitAllThread();

	if (NULL != mpRankJob)
	{
		mpRankJob->clear();
	}

	if (NULL != mpPlatJob)
	{
		mpPlatJob->clear();
	}

	if (NULL != mpLoginJob)
	{
		mpLoginJob->clear();
	}

	for (int i = 0; i < MAX_DB_JOB; ++i)
	{
		if (NULL == mpDBJob[i])
		{
			continue;
		}
		mpDBJob[i]->clear();
	}
	for (int i = 0; i < MAX_MAP_JOB; ++ i)
	{
		if (NULL == mpMapJob[i])
		{
			continue;
		}
		mpMapJob[i]->clear();
	}
}

void CJobManager::run()
{
	mThreadPool.run();
}


/// 检查除了log job是否都已经退出
bool CJobManager::checkOtherJobExit()
{

	if (NULL != mpRankJob)
	{
		if (!mpRankJob->getExited())
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

	for (int i = 0; i < MAX_MAP_JOB; ++i)
	{
		if (NULL == mpMapJob[i])
		{
			continue;
		}
		if (!mpMapJob[i]->getExited())
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

void CJobManager::pushTaskByType(EmJobType eJobType, CInternalMsg* pMsg)
{
	switch (eJobType)
	{
		case emJobTaskType_LocalLog:
		{
			mpLocalLogJob->pushTask(pMsg);
			break;
		}
		case emJobTaskType_Plat:
		{
			mpPlatJob->pushTask(pMsg);
			break;
		}
		case emJobTaskType_Rank:
		{
			mpRankJob->pushTask(pMsg);
			break;
		}
		case emJobTaskType_Login:
		{
			mpLoginJob->pushTask(pMsg);
			break;
		}
		default:
		{
			break;
		}
	}
}

void CJobManager::pushTaskByID(byte nJobID, CInternalMsg* pMsg)
{
	CJob* pJob = (CJob*)mThreadPool.getJobByID(nJobID);
	if (NULL == pJob)
	{
		return;
	}

	pJob->pushTask(pMsg);
}

void CJobManager::pushDBTask(int nUid, CInternalMsg* pMsg)
{
	int nIndex = nUid % MAX_DB_JOB;
	mpDBJob[nIndex]->pushTask(pMsg);
}

void CJobManager::pushBackJob(IJob* pJob)
{
	mThreadPool.pushBackJob(pJob);
}
