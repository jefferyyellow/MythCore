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

/// ��ʼ��
bool CGameServer::initAll()
{
	// ���ȳ�ʼ������������
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
	CRollFileDisplayer* pDefaultFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/gamedefault.log"), 1024000, 10);
	// ΪĬ�ϵ�debug��־���ļ�displayer
	mDefaultLog->AddDisplayer(pDefaultFileDisplayer);
	CLogManager::Inst()->AddDebugLog(mDefaultLog, "default");
#endif

	// ������־���ļ�displayer
	CRollFileDisplayer* pErrorFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/gameerror.log"), 1024000, 10);
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pErrorFileDisplayer);

	// ������־��std displayer
	CStdDisplayer* pDisplayer = new CStdDisplayer();
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pDisplayer);
	
	// ����Ϣ��־���ļ�displayer
	CRollFileDisplayer* pInfoFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/gameinfo.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pInfoFileDisplayer);

	// ��������־���ļ�displayer
	CRollFileDisplayer* pWarnFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/gamewarn.log"), 1024000, 10);
	CLogManager::Inst()->GetWarnLog().AddDisplayer(pWarnFileDisplayer);
	return true;
}


/// ��ʼ�߼�ģ��
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


/// ��ʼ����Ϸ��̬����
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

/// ��ʼ�߳�
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

/// ����
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
		// ������˳�״̬���������Ե�job���Ѿ��˳����
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


/// ��ʼΪ�˳���׼��
void CGameServer::clear()
{
	clearThread();
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

	CLogManager::destroyInst();
}

/// �����߼�ģ��
void CGameServer::clearLogicModule()
{
	CDirtyWord::destroyInst();
	CTimeManager::destroyInst();
	CObjPool::destroyInst();
	CGameServerConfig::destroyInst();
	CInternalMsgPool::destroyInst();
	CMessageFactory::destroyInst();
}

/// ����̬����
void CGameServer::clearStaticData()
{
	CStaticData::clearTemplate();
}

/// �����߳�
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

/// �˳�
void CGameServer::exit()
{
	clear();
}

/// ������log job�Ƿ��Ѿ��˳�
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

/// ������е� job�Ƿ��˳�
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
