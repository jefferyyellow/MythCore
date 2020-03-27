#include "selectserver.h"

namespace Myth
{
	bool CSelectServer::initSocket(int nMaxSocketNum, int nCacheSize, int* pListenPort, int nPortNum, int nBackLogNum)
	{
		CSelectModel::initSocketSystem();
		mCacheSize = nCacheSize;
		mMaxSocketNum = nMaxSocketNum;
		mTcpSocket = new CTcpSocket[nMaxSocketNum];
		if (NULL == mTcpSocket)
		{
			return false;
		}

		mSelectModel = new CSelectModel(mTcpSocket, nMaxSocketNum);
		if (NULL == mSelectModel)
		{
			return false;
		}

		for (int i = 0; i < nPortNum; ++i)
		{
			int nSocketIndex = -1;
			CTcpSocket* pListenSocket = mSelectModel->createListenSocket(NULL, pListenPort[i], nBackLogNum, nSocketIndex);
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

	void CSelectServer::clear()
	{
		for (int i = 0; i < mMaxSocketNum; ++i)
		{
			byte* pBuff = mTcpSocket[i].getRecvBuff();
			if (NULL != pBuff)
			{
				delete[]pBuff;
			}
		}
		delete[]mTcpSocket;
		delete mSelectModel;
	}

	void CSelectServer::receiveMessage()
	{
		mSelectModel->selectAllFd();

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
			if (!FD_ISSET(nFd, &rReadSet))
			{
				continue;
			}
			if (pAllSocket[i].GetListen())
			{
				int nSocketIndex = -1;
				CTcpSocket* pNewSocket = mSelectModel->getFreeSocket(nSocketIndex);
				pAllSocket[i].acceptConnection(pNewSocket);
				if (NULL == pNewSocket)
				{
					// 出错
					continue;
				}
				if (NULL == pNewSocket->getRecvBuff())
				{
					byte* pNewSocketBuff = new byte[mCacheSize];
					if (NULL == pNewSocketBuff)
					{
						clearSocket(pNewSocket);
						continue;
					}
					else
					{
						pNewSocket->setRecvBuff(pNewSocketBuff);
					}
				}

				pNewSocket->setMaxRecvBuffSize(mCacheSize);
				pNewSocket->setRecvBuffSize(0);
				mSelectModel->addNewSocket(pNewSocket, nSocketIndex);
				if (NULL != mNewConnHandle)
				{
					mNewConnHandle(pNewSocket);
				}
			}
			else
			{
				int nResult = pAllSocket[i].recvData(pAllSocket[i].getRecvBuffPoint(), pAllSocket[i].getRecvBuffCapacity());
				if (nResult <= 0)
				{
					CTcpSocket* pSocket = mSelectModel->getSocket(i);
					if (NULL != pSocket)
					{
						// 客户端已经退出
						clearSocket(pSocket);
						if (NULL != mRemoveConnHandle)
						{
							mRemoveConnHandle(pSocket);
						}
					}
					break;
				}
				else
				{
					pAllSocket[i].setRecvBuffSize(pAllSocket[i].getRecvBuffSize() + nResult);
					mReceiveMsgHandle(&(pAllSocket[i]));
				}
			}
		}
	}

	int CSelectServer::sendMessage(CTcpSocket* pSocket, const byte* pBuff, int nBuffSize)
	{
		if (NULL == pBuff || nBuffSize <= 0)
		{
			return -1;
		}
		if (INVALID_SOCKET == pSocket->getSocketFd())
		{
			return -1;
		}
		int nResult = pSocket->sendData(pBuff, nBuffSize);
		if (nResult != nBuffSize)
		{
			clearSocket(pSocket);
			if (NULL != mRemoveConnHandle)
			{
				mRemoveConnHandle(pSocket);
			}
			return nResult;
		}
		return nResult;
	}

	/// 清除socket
	void CSelectServer::clearSocket(CTcpSocket* pSocket)
	{
		mSelectModel->removeSocket(pSocket->getSocketFd());
		pSocket->closeSocket();
	}

}
