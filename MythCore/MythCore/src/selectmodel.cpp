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
	CTcpSocket* CSelectModel::createListenSocket(char* pIP, uint32 uPort, int nListNum, int& rSocketIndex)
	{
		int nSocketIndex = -1;

		CTcpSocket* pNewSocket = getFreeSocket(nSocketIndex);
		if (NULL == pNewSocket)
		{
			// ³ö´í
			return NULL;
		}

		if(INVALID_SOCKET == pNewSocket->createSocket())
		{
			return NULL;
		}

		pNewSocket->setReuseAddr(true);
		pNewSocket->setIP(pIP);
		pNewSocket->setPort(uPort);
		if (pNewSocket->bindPort() < 0)
		{
			//int nError = WSAGetLastError();
			return NULL;
		}
		if (pNewSocket->listenSocket(nListNum) < 0)
		{
			//int nError = WSAGetLastError();
			return NULL;
		}

		pNewSocket->SetListen(1);
		pNewSocket->setNonBlock(true);
		addNewSocket(pNewSocket, nSocketIndex);
		rSocketIndex = nSocketIndex;
		return pNewSocket;
	}

	void CSelectModel::selectAllFd()
	{
		mReadSet = mReadBackSet;
		int nResult = select(mMaxFd + 1, &mReadSet, NULL, NULL, &mSelectTime);
		if (nResult < 0)
		{
			//int nError = WSAGetLastError();
			int i = 0;
		}
	}

	//int CSelectModel::processWrite(int nSocketIndex, char* pBuffer, int nBuffSize)
	//{
	//	if (NULL == pBuffer)
	//	{
	//		return 0;
	//	}
	//	if (nSocketIndex < 0 || nSocketIndex > mMaxSocketIndex)
	//	{
	//		return 0;
	//	}

	//	int nResult = mpAllSocket[nSocketIndex].sendData(pBuffer, nBuffSize);
	//	if (nResult <= 0 || nResult != nBuffSize)
	//	{
	//		int nRemoveFd = mpAllSocket[nSocketIndex].getSocketFd();
	//		mpAllSocket[nSocketIndex].closeSocket();
	//		removeSocket(nRemoveFd);
	//		return nResult;
	//	}
	//	return nResult;
	//}

	//void CSelectModel::processRead()
	//{
	//	int nMaxSocketIndex = mMaxSocketIndex;
	//	for (int i = 0; i <= nMaxSocketIndex; ++ i)
	//	{
	//		int nFd = mpAllSocket[i].getSocketFd();
	//		if (INVALID_SOCKET == nFd)
	//		{
	//			continue;
	//		}
	//		if (FD_ISSET(nFd, &mReadSet))
	//		{
	//			if (mpAllSocket[i].GetListen())
	//			{
	//				int nSocketIndex = -1;
	//				CBuffTcpSocket* pNewSocket = getFreeSocket(nSocketIndex);
	//				mpAllSocket[i].acceptConnection(pNewSocket);
	//				if (nSocketIndex > mMaxSocketIndex)
	//				{
	//					mMaxSocketIndex = nSocketIndex;
	//				}
	//				if (NULL == pNewSocket)
	//				{
	//					// ³ö´í
	//				}
	//				addNewSocket(pNewSocket);
	//			}
	//			else
	//			{
	//				if (NULL != mpAllSocket[i].getRecvBuff())
	//				{
	//					int nResult = mpAllSocket[i].recvData(mpAllSocket[i].getRecvBuffPoint(), mpAllSocket[i].getRecvBuffCapacity());
	//					if (nResult <= 0)
	//					{
	//						int nRemoveFd = mpAllSocket[i].getSocketFd();
	//						mpAllSocket[i].closeSocket();
	//						removeSocket(nRemoveFd);
	//					}
	//					else
	//					{
	//						mpAllSocket[i].setRecvBuffSize(mpAllSocket[i].getRecvBuffSize() + nResult);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

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

	void CSelectModel::addNewSocket(CTcpSocket* pNewSocket, int nSocketIndex)
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
		if (nSocketIndex > mMaxSocketIndex)
		{
			mMaxSocketIndex = nSocketIndex;
		}
#ifdef MYTH_OS_UNIX
		//SetFdNotInherited(nFd);
#endif
	}


	void CSelectModel::removeSocket(SOCKET nRemoveFd)
	{
		FD_CLR(nRemoveFd, &mReadBackSet);
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
				if (nFd == nRemoveFd)
				{
					continue;
				}
				if (INVALID_SOCKET != nFd && nFd > nMaxFd)
				{
					nMaxFd = nFd;
				}
			}

			mMaxFd = nMaxFd;
		}


		for (int nIndex = mMaxSocketIndex; nIndex >= 0; -- nIndex)
		{
			SOCKET nFd = mpAllSocket[nIndex].getSocketFd();
			if (nFd == nRemoveFd)
			{
				continue;
			}
			if (INVALID_SOCKET != nFd)
			{
				mMaxSocketIndex = nIndex;
				break;
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
