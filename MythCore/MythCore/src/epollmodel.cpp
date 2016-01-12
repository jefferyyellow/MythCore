#ifdef MYTH_OS_UNIX
#include "epollmodel.h"
#include <stdio.h>
namespace Myth
{
	int	CEpollModel::initEpollSocket()
	{
		mCtrlEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP;
		mCtrlEvent.data.ptr = NULL;
		mCtrlEvent.data.fd  = -1;

		mpWaitEvents = (struct epoll_event *) malloc(mSocketCapacity * Size(struct epoll_event));
		if(NULL == mpWaitEvents)
		{
			// 出错
		}
		mEpollFd = epoll_create(mSocketCapacity);
		if (mEpollFd < 0)
		{
			// 出错
		}

		mWaitTimeOut = 10;
	}


	int CEpollModel::createListenSocket(char* pIP, uint32 uPort, int nListNum, int nSendBuffSize, int nRecvBuffSize);
	{
		CTcpSocket* pNewSocket = getFreeSocket(nSocketIndex);
		if (NULL == pNewSocket)
		{
			// 出错
			return -1;
		}

		if(INVALID_SOCKET == pNewSocket->createSocket())
		{
			return -1;
		}

		pNewSocket->setIP(pIP);
		pNewSocket->setPort(uPort);
		if(pNewSocket->setNonBlock(true) < 0)
		{
			// 出错
			pNewSocket->setSocketFd(-1);
			return -1;
		}
		if(pNewSocket->setReuseAddr(true) < 0)
		{
			// 出错
			pNewSocket->setSocketFd(-1);
			return -1;
		}
		if(pNewSocket->setLinger(0) < 0)
		{
			// 出错
			pNewSocket->setSocketFd(-1);
			return -1;
		}
		if(pNewSocket->bindPort() < 0)
		{
			pNewSocket->setSocketFd(-1);
			return -1;
		}


		if (nSendBuffSize > 0)
		{
			pNewSocket->setSendBuff(nSendBuffSize);
		}

		if(nRecvBuffSize > 0)
		{
			pNewSocket->setRecvBuff(nRecvBuffSize);
		}
		pNewSocket->listenSocket(nListNum);
		if(addSocket(pNewSocket->getSocketFd()) < 0)
		{
			pNewSocket->setSocketFd(-1);
			return -1;
		}
	}

	int CEpollModel::addSocket(int nFd)
	{
		mCtrlEvent.data.fd = nFd;
		if(epoll_ctl(mEpollFd, EPOLL_CTL_ADD, nfd, &mCtrlEvent) < 0)
		{
			return -1;
		}
		return 0;
	}

	int CEpollModel::delSocket(int nFd)
	{
		mCtrlEvent.data.fd = nFd;
		if (epoll_ctl(mEpollFd, EPOLL_CTL_DEL, nfd, &mCtrlEvent) < 0)
		{
			return -1;
		}
		return 0;
	}

	void CEpollModel::epollWait()
	{
		int nNumFd = epoll_wait(kdpfd, mpWaitEvents, mSocketCapacity, mWaitTimeOut);
		if (nNumFd < 0)
		{
			// 出错，记录错误日志
		}

		struct epoll_event* pEvent = mpWaitEvents;
		int nFd = -1;
		CTcpSocket* pTcpSocket = NULL;
		for (int i = 0, i < nNumFd; i++, cevents++)
		{
			nFd = pEvent->data.fd;
			if (0 > nFd)
			{
				// 出错
				continue;
			}

			// error
			if (0 != (EPOLLERR & pEvent->events))
			{
				// 出错
				continue;
			}
			// 不可读，直接滚蛋
			if (0 == (EPOLLIN & pEvent->events))
			{
				continue;
			}
			if (nFd >= mSocketCapacity)
			{
				continue;
			}
			pTcpSocket = mpAllSocket[nFd];

			// listen socket
			if(pTcpSocket->GetListen())
			{
				SOCKET  nNewFd = accept(mSocketFd, (struct sockaddr*)&serverAddr, &nLen);
				if (nNewFd < 0)
				{
					continue;
				}
				if(mSocketCapacity <= nNewFd)
				{
					shutdown(nNewFd, SHUT_RDWR);
					close(nNewFd);
					continue;
				}

				CTcpSocket* pNewSocket = mpAllSocket[nNewFd];
				pNewSocket->setSocketFd(nNewFd);
				pNewSocket->setPort(ntohs(serverAddr.sin_port));
				pNewSocket->setIP(inet_ntoa(serverAddr.sin_addr));	

				if((pNewSocket->setNonBlock(true)) < 0)
				{
					pNewSocket->closeSocket();
					continue;
				}

				addSocket(pNewSocket->getSocketFd());

			}
			else
			{
				// receive socket data
				char acBuffer[256] = {0};
				pTcpSocket->recvData(acBuffer, sizeof(acBuffer));
				printf("%s\n", acBuffer);
			}
		}
	}
}
#endif //  MYTH_OS_UNIX

