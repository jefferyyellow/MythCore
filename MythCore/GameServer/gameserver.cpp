#include "gameserver.h"
#include "logmanager.h"
#include "log.h"
#include "logdisplayer.h"

/// ��ʼ��
bool CGameServer::init()
{
	bool bResult = initLog();
	if (!bResult)
	{
		return false;
	}

	bResult = initShareMemory();
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
#endif

	// Ĭ�ϵ�debug��־
	CRollFileDisplayer* pDefaultFileDisplayer = new CRollFileDisplayer(const_cast<char*>("default.log"), 1024000, 10);
	// ΪĬ�ϵ�debug��־���ļ�displayer
	mDefaultLog->AddDisplayer(pDefaultFileDisplayer);
	CLogManager::Inst()->AddDebugLog(mDefaultLog, "default");

	// ������־���ļ�displayer
	CRollFileDisplayer* pErrorFileDisplayer = new CRollFileDisplayer(const_cast<char*>("error.log"), 1024000, 10);
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pErrorFileDisplayer);

	// ������־��std displayer
	CStdDisplayer* pDisplayer = new CStdDisplayer();
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pDisplayer);
	
	// ����Ϣ��־���ļ�displayer
	CRollFileDisplayer* pInfoFileDisplayer = new CRollFileDisplayer(const_cast<char*>("info.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pInfoFileDisplayer);

	// ��������־���ļ�displayer
	CRollFileDisplayer* pWarnFileDisplayer = new CRollFileDisplayer(const_cast<char*>("warn.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pWarnFileDisplayer);
	return true;
}

/// ��ʼ���ܵ�
bool CGameServer::initShareMemory()
{
	// һ��CShareMemory������
	int nShareMemorySize = sizeof(CShareMemory);
	// ����CSocketStream������
	nShareMemorySize += 2 * sizeof(CSocketStream);
	// ������ʵ���ڴ�����
	nShareMemorySize += 2 * PIPE_SIZE;

	bool bCreate = true;
	uint8* pSharePoint = CShareMemory::createShareMemory(37345234, nShareMemorySize, bCreate);
	if (NULL == pSharePoint)
	{
		return false;
	}
	// ��ʼ��
	mShareMemory = (CShareMemory*)pSharePoint;
	if (bCreate)
	{
		mShareMemory->setShmPoint(pSharePoint);
		mShareMemory->setShmKey(37345234);
		mShareMemory->setShmSize(nShareMemorySize);
	}


	pSharePoint += sizeof(CShareMemory);

	// ��ʼ��Tcp2Server�����ڴ�
	mTcp2ServerMemory = (CSocketStream*)pSharePoint;
	if (bCreate)
	{
		mTcp2ServerMemory->Initialize(pSharePoint + sizeof(CSocketStream), PIPE_SIZE);
	}

	pSharePoint += sizeof(CSocketStream) + PIPE_SIZE;
	// ��ʼ��Server2Tcp�����ڴ�
	mServer2TcpMemory = (CSocketStream*)pSharePoint;
	if (bCreate)
	{
		mServer2TcpMemory->Initialize(pSharePoint + sizeof(CSocketStream), PIPE_SIZE);
	}
	return true;
}

/// ��ʼ�߳�
bool CGameServer::initThread()
{
	mThreadPool = new Myth::CThreadPool(10);
	if (NULL == mThreadPool)
	{
		return false;
	}



	return true;
}

/// ����
void CGameServer::run()
{
	while (true)
	{
		processClientMessage();
	}
}

/// ����ǰ����Ϣ
void CGameServer::processClientMessage()
{
	int nMessageLen = MAX_SOCKET_BUFF_SIZE;
	int nResult = 0;
	for (int i = 0; i < 2000; ++i)
	{
		nResult = mTcp2ServerMemory->GetHeadPacket((uint8*)mBuffer, nMessageLen);
		if (nResult < 0)
		{
			break;
		}

		if (nMessageLen <= 0)
		{
			break;
		}

		char* pTemp = mBuffer;
		CExchangeHead* pExchangeHead = (CExchangeHead*)mBuffer;

		pTemp += sizeof(CExchangeHead);
		nMessageLen -= sizeof(CExchangeHead);

		short nLength = *(short*)pTemp;
		if (nLength != nMessageLen)
		{
			break;
		}

		pTemp += sizeof(short);
		nMessageLen -= sizeof(short);

		short nMessageID = *(short*)pTemp;
		pTemp += sizeof(short);
		nMessageLen -= sizeof(short);

		char acBuffer[1024] = {0};
		strncpy(acBuffer, pTemp + 2, nMessageLen - 2);
		printf(acBuffer);
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
	uint8 nErrorDisplaySize = CLogManager::Inst()->GetErrorLog().GetDisplayerSize();
	for (int i = 0; i < nErrorDisplaySize; ++i)
	{
		delete CLogManager::Inst()->GetErrorLog().GetDisplayer(i);
	}
	CLogManager::Inst()->GetErrorLog().SetDisplayerSize(0);

	// ɾ����Ϣ��־��displayer
	uint8 nInfoDisplaySize = CLogManager::Inst()->GetInfoLog().GetDisplayerSize();
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