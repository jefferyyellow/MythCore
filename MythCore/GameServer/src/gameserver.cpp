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

/// ��ʼ��
bool CGameServer::initAll()
{
	// ���ȳ�ʼ������������
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

	bResult = mpJobManager->initThread();
	if (!bResult)
	{
		return false;
	}
	return true;
}

/// ��ʼ��־
bool CGameServer::initLog()
{
	CLogManager* pLogManger = CLogManager::createInst();
	if (NULL == pLogManger)
	{
		printf("create log manager failure");
		return false;
	}

#ifdef __DEBUG__
	// Ĭ����־
	mDefaultLog = new CLog();
	if (NULL == mDefaultLog)
	{
		printf("create default log failure");
		return false;
	}

	// Ĭ�ϵ�debug��־
	CRollFileDisplayer* pDefaultFileDisplayer = new CRollFileDisplayer(const_cast<char*>("log/gamedefault.log"), 1024000, 10);
	// ΪĬ�ϵ�debug��־���ļ�displayer
	mDefaultLog->AddDisplayer(pDefaultFileDisplayer);
	CLogManager::Inst()->AddDebugLog(mDefaultLog, "default");
#endif

	// ������־���ļ�displayer
	CRollFileDisplayer* pErrorFileDisplayer = new CRollFileDisplayer(const_cast<char*>("log/gameerror.log"), 1024000, 10);
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pErrorFileDisplayer);

	// ������־��std displayer
	CStdDisplayer* pDisplayer = new CStdDisplayer();
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pDisplayer);
	
	// ����Ϣ��־���ļ�displayer
	CRollFileDisplayer* pInfoFileDisplayer = new CRollFileDisplayer(const_cast<char*>("log/gameinfo.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pInfoFileDisplayer);

	// ��������־���ļ�displayer
	CRollFileDisplayer* pWarnFileDisplayer = new CRollFileDisplayer(const_cast<char*>("log/gamewarn.log"), 1024000, 10);
	CLogManager::Inst()->GetWarnLog().AddDisplayer(pWarnFileDisplayer);
	return true;
}


/// ��ʼ�߼�ģ��
bool CGameServer::initLogicModule()
{
	CPerfManager::createInst();
	CMessageFactory::createInst();
	CInternalMsgPool::createInst();
	CGameServerConfig::createInst();
	CGameLogicConfig::createInst();
	CObjPool::createInst();
	CTimeManager::createInst();
	CDirtyWord::createInst();
	CJobManager::createInst();
	return true;
}


/// ��ʼ����Ϸ��̬����
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


/// ����
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
			// ������
			if (mMinuteTimer.elapse((int)(tTimeNow - mLastTime)))
			{
				logPerf();
			}
			mLastTime = tTimeNow;
		}

		// ������˳�״̬���������Ե�job���Ѿ��˳����
		if (mExit && mpJobManager->checkAllJobExit())
		{
			break;
		}
	}
}


/// ��ʼΪ�˳���׼��
void CGameServer::clear()
{
	mpJobManager->clearThread();
	clearStaticData();
	clearLog();
	clearLogicModule();
}

/// ������־��Դ
void CGameServer::clearLog()
{
	// ɾ��������־��displayer
	byte nErrorDisplaySize = CLogManager::Inst()->GetErrorLog().GetDisplayerSize();
	for (int i = 0; i < nErrorDisplaySize; ++i)
	{
		delete CLogManager::Inst()->GetErrorLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetErrorLog().SetDisplayerSize(0);

	// ɾ����Ϣ��־��displayer
	byte nInfoDisplaySize = CLogManager::Inst()->GetInfoLog().GetDisplayerSize();
	for (int i = 0; i < nInfoDisplaySize; ++ i)
	{
		delete CLogManager::Inst()->GetInfoLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetInfoLog().SetDisplayerSize(0);

	// ɾ��������־��displayer
	int nWarnDisplaySize = CLogManager::Inst()->GetWarnLog().GetDisplayerSize();
	for (int i = 0; i < nWarnDisplaySize; ++ i)
	{
		delete CLogManager::Inst()->GetWarnLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetWarnLog().SetDisplayerSize(0);

#ifdef __DEBUG__
	// ɾ��Ĭ����־��displayer
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

/// �����߼�ģ��
void CGameServer::clearLogicModule()
{
	CDirtyWord::destroyInst();
	CTimeManager::destroyInst();
	CObjPool::destroyInst();
	CGameLogicConfig::destroyInst();
	CGameServerConfig::destroyInst();
	CInternalMsgPool::destroyInst();
	CMessageFactory::destroyInst();
	CPerfManager::destroyInst();
}

/// ����̬����
void CGameServer::clearStaticData()
{
	CStaticData::clearTemplate();
}



/// �˳�
void CGameServer::exit()
{
	clear();
}


/// ��¼���е�Perf�ļ�¼
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

