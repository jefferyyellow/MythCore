#include "selectmodel.h"
#include <stdio.h>
namespace Myth
{
	int CSelectModel::initSocketSystem()
	{
#ifdef MYTH_OS_WINDOWS
		WSADATA  Ws;
		if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
		{
			return -1;
		}
#endif // MYTH_OS_WINDOWS
		return 0;
	}
	int CSelectModel::createListenSocket(char* pIP, uint32 uPort, int nListNum)
	{
		int nSocketIndex = -1;

		CTcpSocket* pNewSocket = getFreeSocket(nSocketIndex);
		if (NULL == pNewSocket)
		{
			// ³ö´í
			return -1;
		}

		if(INVALID_SOCKET == pNewSocket->createSocket())
		{
			return -1;
		}

		pNewSocket->setIP(pIP);
		pNewSocket->setPort(uPort);
		if (pNewSocket->bindPort() < 0)
		{
			int nError = WSAGetLastError();
			return -1;
		}
		if (pNewSocket->listenSocket(nListNum) < 0)
		{
			int nError = WSAGetLastError();
			return -1;
		}
		if (nSocketIndex > mMaxSocketIndex)
		{
			mMaxSocketIndex = nSocketIndex;
		}
		pNewSocket->SetListen(1);
		addNewSocket(pNewSocket);
		return 0;
	}

	void CSelectModel::selectAllFd()
	{
		mReadSet = mReadBackSet;
		int nResult = select(mMaxFd + 1, &mReadSet, NULL, NULL, &mSelectTime);
		if (nResult < 0)
		{
			int nError = WSAGetLastError();
			int i = 0;
		}
	}

	void CSelectModel::processRead()
	{
		int nMaxSocketIndex = mMaxSocketIndex;
		for (int i = 0; i <= nMaxSocketIndex; ++ i)
		{
			int nFd = mpAllSocket[i].getSocketFd();
			if (INVALID_SOCKET == nFd)
			{
				continue;
			}
			if (FD_ISSET(nFd, &mReadSet))
			{
				if (mpAllSocket[i].GetListen())
				{
					int nSocketIndex = -1;
					CTcpSocket* pNewSocket = getFreeSocket(nSocketIndex);
					mpAllSocket[i].acceptConnection(pNewSocket);
					if (nSocketIndex > mMaxSocketIndex)
					{
						mMaxSocketIndex = nSocketIndex;
					}
					if (NULL == pNewSocket)
					{
						// ³ö´í
					}
					addNewSocket(pNewSocket);
				}
				else
				{
					char acBuffer[128] = { 0 };
					int nResult = mpAllSocket[i].recvData(acBuffer, sizeof(acBuffer));
					if (nResult < 0)
					{ 
						int nRemoveFd = mpAllSocket[i].getSocketFd();
						mpAllSocket[i].closeSocket();
						removeSocket(nRemoveFd);
					}
					else
					{
						printf("%s\n", acBuffer);
					}
				}
			}
		}
	}

	CTcpSocket*	CSelectModel::getFreeSocket(int &rSocketIndex)
	{
		int nIndex = findFreeSocketIndex();
		if (nIndex < 0)
		{
			return NULL;
		}

		if (nIndex >= mSocketCapacity)
		{
			return NULL;
		}
		rSocketIndex = nIndex;
		return &mpAllSocket[nIndex];
	}

	void CSelectModel::addNewSocket(CTcpSocket* pNewSocket)
	{
		if (NULL == pNewSocket)
		{
			return;
		}
		SOCKET nFd = pNewSocket->getSocketFd();
		if (nFd > mMaxFd)
		{
			mMaxFd = nFd;
		}
		FD_SET(nFd, &mReadBackSet);
	}


	void CSelectModel::removeSocket(SOCKET nRemoveFd)
	{
		if(nRemoveFd >= mMaxFd)
		{
#ifdef MYTH_OS_WINDOWS
			SOCKET nMaxFd = 0;
#else
			SOCKET nMaxFd = -1;
#endif // DEBUG
			for (int i = 0; i <= mMaxSocketIndex; ++ i)
			{
				SOCKET nFd = mpAllSocket[i].getSocketFd();
				if (INVALID_SOCKET != nFd && nFd > nMaxFd)
				{
					mMaxFd = nFd;
				}
			}
		}
		for (int nIndex = mMaxSocketIndex; nIndex >= 0; -- nIndex)
		{
			if (INVALID_SOCKET != mpAllSocket[nIndex].getSocketFd())
			{
				mMaxSocketIndex = nIndex;
			}
		}
	}

	int CSelectModel::findFreeSocketIndex()
	{
		for (int i = 0; i < mSocketCapacity; ++i)
		{
			if (INVALID_SOCKET == mpAllSocket[i].getSocketFd())
			{
				return i;
			}
		}

		return -1;
	}
}