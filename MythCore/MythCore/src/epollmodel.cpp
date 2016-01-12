#ifdef MYTH_OS_UNIX
#include "epollmodel.h"
#include "tcpsocket.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
namespace Myth
{
	int	CEpollModel::initEpollSocket()
	{
		mCtrlEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP;
		mCtrlEvent.data.ptr = NULL;
		mCtrlEvent.data.fd  = -1;

		mpWaitEvents = (struct epoll_event *) malloc(mSocketCapacity * sizeof(struct epoll_event));
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


	int CEpollModel::createListenSocket(char* pIP, uint32 uPort, int nListNum, int nSendBuffSize, int nRecvBuffSize)
	{
		SOCKET nNewFd = socket(AF_INET, SOCK_STREAM, 0);
		if(INVALID_SOCKET == nNewFd)
		{
			return -1;
		}
		if (nNewFd >= mSocketCapacity)
		{
			shutdown(nNewFd, SHUT_RDWR);
			close(nNewFd);
			return -1;
		}

		CTcpSocket& rNewSocket = mpAllSocket[nNewFd];

		rNewSocket.setSocketFd(nNewFd);
		rNewSocket.setIP(pIP);
		rNewSocket.setPort(uPort);
		if(rNewSocket.setNonBlock(true) < 0)
		{
			// 出错
			rNewSocket.setSocketFd(-1);
			return -1;
		}
		if(rNewSocket.setReuseAddr(true) < 0)
		{
			// 出错
			rNewSocket.setSocketFd(-1);
			return -1;
		}
		if(rNewSocket.setLinger(0) < 0)
		{
			// 出错
			rNewSocket.setSocketFd(-1);
			return -1;
		}
		if(rNewSocket.bindPort() < 0)
		{
			rNewSocket.setSocketFd(-1);
			return -1;
		}


		if (nSendBuffSize > 0)
		{
			rNewSocket.setSendBuffSize(nSendBuffSize);
		}

		if(nRecvBuffSize > 0)
		{
			rNewSocket.setRecvBuffSize(nRecvBuffSize);
		}
		rNewSocket.listenSocket(nListNum);
		if(addSocket(rNewSocket.getSocketFd()) < 0)
		{
			rNewSocket.setSocketFd(-1);
			return -1;
		}
	}

	int CEpollModel::addSocket(int nFd)
	{
		mCtrlEvent.data.fd = nFd;
		if(epoll_ctl(mEpollFd, EPOLL_CTL_ADD, nFd, &mCtrlEvent) < 0)
		{
			return -1;
		}
		return 0;
	}

	int CEpollModel::delSocket(int nFd)
	{
		mCtrlEvent.data.fd = nFd;
		if (epoll_ctl(mEpollFd, EPOLL_CTL_DEL, nFd, &mCtrlEvent) < 0)
		{
			return -1;
		}
		return 0;
	}

	void CEpollModel::epollWait()
	{
		int nNumFd = epoll_wait(mEpollFd, mpWaitEvents, mSocketCapacity, mWaitTimeOut);
		if (nNumFd < 0)
		{
			// 出错，记录错误日志
		}

		struct epoll_event* pEvent = mpWaitEvents;
		int nFd = -1;
		for (int i = 0; i < nNumFd; i++, pEvent++)
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
			CTcpSocket& rTcpSocket = mpAllSocket[nFd];

			// listen socket
			if(rTcpSocket.GetListen())
			{
				sockaddr_in serverAddr;
				socklen_t nLen = sizeof(serverAddr);
				SOCKET  nNewFd = accept(rTcpSocket.getSocketFd(), (struct sockaddr*)&serverAddr, &nLen);
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

				CTcpSocket& rNewSocket = mpAllSocket[nNewFd];
				rNewSocket.setSocketFd(nNewFd);
				rNewSocket.setPort(ntohs(serverAddr.sin_port));
				rNewSocket.setIP(inet_ntoa(serverAddr.sin_addr));	

				if((rNewSocket.setNonBlock(true)) < 0)
				{
					rNewSocket.closeSocket();
					continue;
				}

				addSocket(rNewSocket.getSocketFd());

			}
			else
			{
				// receive socket data
				char acBuffer[256] = {0};
				rTcpSocket.recvData(acBuffer, sizeof(acBuffer));
				printf("%s\n", acBuffer);
			}
		}
	}
}
#endif //  MYTH_OS_UNIX

