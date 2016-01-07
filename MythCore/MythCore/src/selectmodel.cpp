#include "selectmodel.h"
namespace Myth
{
	void CSelectModel::createListenSocket(char* pIP, uint32 uPort, int nListNum)
	{
		int nSocketIndex = -1;

		CTcpSocket* pNewSocket = getFreeSocket(nSocketIndex);
		if (NULL == pNewSocket)
		{
			// ³ö´í
			return;
		}
		if (nSocketIndex > mMaxSocketIndex)
		{
			mMaxSocketIndex = nSocketIndex;
		}
		if(INVALID_SOCKET == pNewSocket->createSocket())
		{
			return;
		}

		pNewSocket->bindPort();
		pNewSocket->listenSocket(nListNum);
	}

	void CSelectModel::selectAllFd()
	{
		mReadSet = mReadBackSet;
		int nResult = select(mMaxFd + 1, &mReadSet, NULL, NULL, &mSelectTime);
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
				}
				else
				{
					mpAllSocket[i].processRead();
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

		if (nIndex >= mSocketNum)
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
	}


	void CSelectModel::removeSocket(CTcpSocket* pRemoveSocket)
	{
		if (NULL == pRemoveSocket)
		{
			return;
		}

		SOCKET nRemoveFd = pRemoveSocket->getSocketFd();
		if(nRemoveFd >= mMaxFd)
		{
			SOCKET nMaxFd = -1;
			for (int i = 0; i < mMaxSocketIndex; ++ i)
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
		for (int i = 0; i < mSocketNum; ++ i)
		{
			if (INVALID_SOCKET == mpAllSocket[i].getSocketFd())
			{
				return i;
			}
		}

		return -1;
	}
}