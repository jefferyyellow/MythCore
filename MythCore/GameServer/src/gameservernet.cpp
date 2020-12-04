#include "gameservernet.h"
#include "clientsocketjob.h"
#include "jobmanager.h"
bool CGameServerNet::initSocket(int nListenSocketNum, int nCacheSize, int* pListenPort, int nPortNum, int nBackLogNum)
{
	mpTcpSocket = new CTcpSocket[nListenSocketNum];
	if (NULL == mpTcpSocket)
	{
		return false;
	}

	mSelectModel.setAllSocket(mpTcpSocket);
	mSelectModel.setSocketCapacity(nListenSocketNum);

	for (int i = 0; i < nPortNum; ++i)
	{
		int nSocketIndex = -1;
		CTcpSocket* pListenSocket = mSelectModel.createListenSocket(NULL, pListenPort[i], nBackLogNum, nSocketIndex);
		if (NULL == pListenSocket)
		{
			return false;
		}

		pListenSocket->setNonBlock(true);
		pListenSocket->setSendBuffSizeOption(nCacheSize);
		pListenSocket->setRecvBuffSizeOption(nCacheSize);
	}

	return true;
}

void CGameServerNet::receiveMessage()
{
	mSelectModel.selectAllFd();

	int nMaxSocketIndex = mSelectModel.getMaxSocketIndex();
	CTcpSocket* pAllSocket = mSelectModel.getAllSocket();
	fd_set& rReadSet = mSelectModel.getReadSet();
	for (int i = 0; i <= nMaxSocketIndex; ++i)
	{
		int nFd = pAllSocket[i].getSocketFd();
		if (INVALID_SOCKET == nFd)
		{
			continue;
		}
		if (!FD_ISSET(nFd, &rReadSet))
		{
			continue;
		}
		if (pAllSocket[i].GetListen())
		{
			CClientSocketJob* pSocketJob = new CClientSocketJob;
			if (NULL != pSocketJob)
			{
				pAllSocket[i].acceptConnection(&(pSocketJob->getTcpSocket()));
				pSocketJob->initSocket();
				CJobManager::Inst()->pushBackJob(pSocketJob);
			}
		}
	}
}