#include "tcpserver.h"




CTcpServer::CTcpServer()
{

}

CTcpServer::~CTcpServer()
{

}

/// ��ʼ��
bool CTcpServer::init()
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

	bResult = initSocket();

	return true;
}

/// ��ʼ��־
bool CTcpServer::initLog()
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
bool CTcpServer::initShareMemory()
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

/// ��ʼ��Socket
bool CTcpServer::initSocket()
{
	CSelectModel::initSocketSystem();

	mTcpSocket = new CTcpSocket[MAX_SOCKET_NUM];
	if (NULL == mTcpSocket)
	{
		return false;
	}
	mSelectModel = new CSelectModel(mTcpSocket, MAX_SOCKET_NUM);
	if (NULL == mSelectModel)
	{
		return false;
	}
	
	CTcpSocket* pListenSocket = mSelectModel->createListenSocket(NULL, 6688, 5);
	if (NULL == pListenSocket)
	{
		return false;
	}
	pListenSocket->setNonBlock(true);
	pListenSocket->setSendBuffSizeOption(8192);
	pListenSocket->setRecvBuffSizeOption(8192);

	return true;
}

/// ����
void CTcpServer::run()
{
	while (true)
	{
		mSelectModel->selectAllFd();
		receiveMessage();
	}
	
}

/// ������Ϣ
void CTcpServer::receiveMessage()
{
	int nMaxSocketIndex = mSelectModel->getMaxSocketIndex();
	CTcpSocket* pAllSocket = mSelectModel->getAllSocket();
	fd_set& rReadSet = mSelectModel->getReadSet();
	for (int i = 0; i <= nMaxSocketIndex; ++i)
	{
		int nFd = pAllSocket[i].getSocketFd();
		if (INVALID_SOCKET == nFd)
		{
			continue;
		}
		if (FD_ISSET(nFd, &rReadSet))
		{
			if (pAllSocket[i].GetListen())
			{
				printf("�����ӳɹ�����\n");
				int nSocketIndex = -1;
				CTcpSocket* pNewSocket = mSelectModel->getFreeSocket(nSocketIndex);
				pAllSocket[i].acceptConnection(pNewSocket);
				if (nSocketIndex > mSelectModel->getMaxSocketIndex())
				{
					mSelectModel->setMaxSocketIndex(nSocketIndex);
				}
				if (NULL == pNewSocket)
				{
					// ����
				}
				CTcpSocketBuff* pNewSocketBuff = mSocketBuffPool.allocate();
				if (NULL == pNewSocketBuff)
				{
					// ����
				}
				pNewSocket->setRecvBuff(pNewSocketBuff->mData);
				pNewSocket->setMaxRecvBuffSize(MAX_SOCKET_BUFF_SIZE);
				pNewSocket->setRecvBuffSize(0);
				mSelectModel->addNewSocket(pNewSocket);
				printf("���ӳɹ���\n");
			}
			else
			{
				int nResult = pAllSocket[i].recvData(pAllSocket[i].getRecvBuffPoint(), pAllSocket[i].getRecvBuffCapacity());
				if (nResult <= 0)
				{
					// �ͻ����Ѿ��˳�
					int nRemoveFd = pAllSocket[i].getSocketFd();
					pAllSocket[i].closeSocket();
					mSelectModel->removeSocket(nRemoveFd);
					break;
				}
				else
				{
					pAllSocket[i].setRecvBuffSize(pAllSocket[i].getRecvBuffSize() + nResult);
					onReceiveMessage(&(pAllSocket[i]), i);
				}
			}
		}
	}
}

// �������յ�һ��ǰ����Ϣ
void CTcpServer::onReceiveMessage(CTcpSocket* pSocket, int nIndex)
{
	if (NULL == pSocket)
	{
		return;
	}

	int nBuffSize = pSocket->getRecvBuffSize();
	char* pBuffer = pSocket->getRecvBuff();

	int nTotalSize = 0;
	while (true)
	{
		if (nBuffSize < 6)
		{
			return;
		}

		short nMessageLen = *(short*)pBuffer;
		if (nMessageLen < 6 || nMessageLen > MAX_TCPBUFF_LEN)
		{
			// ����
			return;
		}

		// ��Ϣ����û�н�����ȫ
		if (nBuffSize < nMessageLen)
		{
			return;
		}


		mExchangeHead.mTcpIndex = nIndex;
		mExchangeHead.mTcpState = 0;

		memcpy(mBuffer, &mExchangeHead, sizeof(mExchangeHead));
		memcpy(mBuffer + sizeof(mExchangeHead), pBuffer, nMessageLen);
		printf("%s\n", pBuffer + 6);
		mTcp2ServerMemory->PushPacket((uint8*)mBuffer, nMessageLen + sizeof(mExchangeHead));

		pBuffer += nMessageLen;
		nBuffSize -= nMessageLen;
	}

	int nSendSize = pSocket->getRecvBuffSize() - nBuffSize;
	pSocket->setRecvBuffSize(nBuffSize);
	pSocket->resetRecvBuffPoint(nSendSize);
}

// ���Ϳͻ�����Ϣ
void CTcpServer::sendMessage()
{
	int nMessageLen = MAX_SOCKET_BUFF_SIZE;
	int nResult = 0;
	for (int i = 0; i < MAX_SEND_PACKAGE_ONCE; ++ i)
	{
		nResult = mServer2TcpMemory->GetHeadPacket((uint8*)mBuffer, nMessageLen);
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

		int nTcpIndex = pExchangeHead->mTcpIndex;
		int nResult = mSelectModel->processWrite(nTcpIndex, pTemp, nMessageLen);
		if (nResult == nMessageLen && pExchangeHead->mTcpState == emTcpState_Close)
		{
			CTcpSocket* pSocket = mSelectModel->getSocket(nTcpIndex);
			if (NULL != pSocket)
			{
				int nRemoveFd = pSocket->getSocketFd();
				pSocket->closeSocket();
				mSelectModel->removeSocket(nRemoveFd);
			}
		}
	}
}

/// ��ʼΪ�˳���׼��
void CTcpServer::clear()
{

}

/// �˳�
void CTcpServer::exit()
{

}