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

/// ��ʼ��
bool CGameServer::init()
{
	// ���ȳ�ʼ������������
	mCurrTime = time(NULL);
	mTickCount = GetTickCount64();
	srand((unsigned int)mCurrTime);

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
	CLogManager* pLogManger = CLogManager::CreateInst();
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
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pWarnFileDisplayer);
	return true;
}


/// ��ʼ�߼�ģ��
bool CGameServer::initLogicModule()
{
	CMessageFactory::CreateInst();
	CInternalMsgPool::CreateInst();
	CGameServerConfig::CreateInst();
	CObjPool::CreateInst();
	CTimeManager::CreateInst();

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

	//bResult = CStaticData::loadFromFile("gameserverconfig/template/template_server.dat");
	//if (!bResult)
	//{
	//	return bResult;
	//}
	return true;
}

/// ��ʼ�߳�
bool CGameServer::initThread()
{
	mThreadPool.init(10);

	//Sleep(3000);
	
	if (0 != mDBJob.init(CGameServerConfig::Inst()->getDBHost(),
		CGameServerConfig::Inst()->getDBUserName(), CGameServerConfig::Inst()->getDBPasswd(),
		CGameServerConfig::Inst()->getDefaultDataBase(), CGameServerConfig::Inst()->getDBPort(),
		NULL))
	{
		return false;
	}

	bool bResult = mSceneJob.init(MAX_SCENE_DB_BUFFER_SIZE);
	if (!bResult)
	{
		return false;
	}
	mDBJob.setBuffer(MAX_DB_JOB_BUFFER_SIZE);

	printf("initThread\n");
	mThreadPool.pushBackJob(&mSceneJob);
	mThreadPool.pushBackJob(&mDBJob);
	mThreadPool.pushBackJob(&mLocalLogJob);
	return true;
}

/// ����
void CGameServer::run()
{
	//LOG_ERROR("Hello World");
	//MYTH_ASSERT(1,;);
	//MYTH_ASSERT_INFO(1,;,"I love you macro!");
	while (true)
	{
		mCurrTime = time(NULL);
		mTickCount = GetTickCount64();
		//printf("*dddd*");
		mThreadPool.run();
#ifdef MYTH_OS_WINDOWS
		Sleep(1);
#else
		struct timespec tv;
		tv.tv_sec = 0;
		tv.tv_nsec = 1000;
		nanosleep(&tv, NULL);
#endif
	}
}


/// ��ʼΪ�˳���׼��
void CGameServer::clear()
{

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
}

/// �˳�
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

void CGameServer::pushDBTask(int nUid, byte* pData, int nDataLength)
{
	mDBJob.pushBackJobData(pData, nDataLength);
}
