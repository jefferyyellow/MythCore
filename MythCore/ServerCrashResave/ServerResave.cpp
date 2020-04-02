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

/// ��ʼ��
bool CServerResave::initAll()
{
	// ���ȳ�ʼ������������
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

	// û�����������
	if (bCreate)
	{
		CShareMemory::destroyShareMemory(nShareMemoryKey, pSharePoint);
		return false;
	}

	CObjPool::Inst()->setShareMemory(pSharePoint);
	ResetPlayerSaveStatus();
	return true;
}

/// ��ʼ��־
bool CServerResave::initLog()
{
	CLogManager* pLogManger = CLogManager::createInst();
	if (NULL == pLogManger)
	{
		printf("create log manager failure");
		return false;
	}


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
bool CServerResave::initLogicModule()
{
	CTimeManager::createInst();
	CInternalMsgPool::createInst();
	CGameServerConfig::createInst();
	CObjPool::createInst();
	CJobManager::createInst();
	// ע�⣬����CSceneJob����job��������ȥ����
	CSceneJob::createInst();
	CPropertyModule::createInst();
	CDBModule::createInst();

	return true;
}


/// ��ʼ����Ϸ��̬����
bool CServerResave::initStaticData()
{
	bool bResult = CGameServerConfig::Inst()->loadGameServerConfigFromXml("config/gameserverconfig.xml");
	if (!bResult)
	{
		return bResult;
	}
	return true;
}


/// ����
void CServerResave::run()
{
	static int i = 0;
	while (true)
	{
		CTimeManager::Inst()->setCurrTime(time(NULL));
		time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
		if (tTimeNow != mLastTime)
		{
			// ������
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
		// ������˳�״̬���������Ե�job���Ѿ��˳����
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
	// �˳�
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
		// �Ѿ��ڱ�������
		if (emSaveStatus_None != pPlayer->getSaveStatus())
		{
			// û�б������
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

	// �Ѿ��������
	if (nCount <= 0 && nSavingCount <= 0)
	{
		printf("�������");
		setExit(true);
		CSceneJob::Inst()->setExited(true);
	}
}


/// ��ʼΪ�˳���׼��
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

/// ������־��Դ
void CServerResave::clearLog()
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
	for (int i = 0; i < nInfoDisplaySize; ++i)
	{
		delete CLogManager::Inst()->GetInfoLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetInfoLog().SetDisplayerSize(0);

	// ɾ��������־��displayer
	int nWarnDisplaySize = CLogManager::Inst()->GetWarnLog().GetDisplayerSize();
	for (int i = 0; i < nWarnDisplaySize; ++i)
	{
		delete CLogManager::Inst()->GetWarnLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetWarnLog().SetDisplayerSize(0);

	CLogManager::destroyInst();
}

/// �����߼�ģ��
void CServerResave::clearLogicModule()
{
	CObjPool::destroyInst();
	CGameServerConfig::destroyInst();
	CInternalMsgPool::destroyInst();
}

/// ����̬����
void CServerResave::clearStaticData()
{
}



/// �˳�
void CServerResave::exit()
{
	clear();
}
