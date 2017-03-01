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


	CTcpSocket* CEpollModel::createListenSocket(char* pIP, uint32 uPort, int nListNum, int nSendBuffSize, int nRecvBuffSize)
	{
		SOCKET nNewFd = socket(AF_INET, SOCK_STREAM, 0);
		if(INVALID_SOCKET == nNewFd)
		{
			return NULL;
		}
		if (nNewFd >= mSocketCapacity)
		{
			shutdown(nNewFd, SHUT_RDWR);
			close(nNewFd);
			return NULL;
		}

		CTcpSocket& rNewSocket = mpAllSocket[nNewFd];

		rNewSocket.setSocketFd(nNewFd);
		rNewSocket.setIP(pIP);
		rNewSocket.setPort(uPort);
		if(rNewSocket.setNonBlock(true) < 0)
		{
			// 出错
			rNewSocket.setSocketFd(-1);
			return NULL;
		}
		if(rNewSocket.setReuseAddr(true) < 0)
		{
			// 出错
			rNewSocket.setSocketFd(-1);
			return NULL;
		}
		if(rNewSocket.setLinger(0) < 0)
		{
			// 出错
			rNewSocket.setSocketFd(-1);
			return NULL;
		}
		if(rNewSocket.bindPort() < 0)
		{
			rNewSocket.setSocketFd(-1);
			return NULL;
		}


		//if (nSendBuffSize > 0)
		//{
		//	rNewSocket.setSendBuffSize(nSendBuffSize);
		//}

		//if(nRecvBuffSize > 0)
		//{
		//	rNewSocket.setRecvBuffSize(nRecvBuffSize);
		//}
		rNewSocket.listenSocket(nListNum);
		if(addSocket(rNewSocket.getSocketFd()) < 0)
		{
			rNewSocket.setSocketFd(-1);
			return NULL;
		}

		return &rNewSocket;
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

	CTcpSocket* CEpollModel::acceptConnection(CTcpSocket& rListSocket)
	{
		sockaddr_in serverAddr;
		socklen_t nLen = sizeof(serverAddr);
		SOCKET  nNewFd = accept(rListSocket.getSocketFd(), (struct sockaddr*)&serverAddr, &nLen);
		if (nNewFd < 0)
		{
			return NULL;
		}
		if (mSocketCapacity <= nNewFd)
		{
			shutdown(nNewFd, SHUT_RDWR);
			close(nNewFd);
			return NULL;
		}

		CTcpSocket& rNewSocket = mpAllSocket[nNewFd];
		rNewSocket.setSocketFd(nNewFd);
		rNewSocket.setPort(ntohs(serverAddr.sin_port));
		rNewSocket.setIP(inet_ntoa(serverAddr.sin_addr));

		if ((rNewSocket.setNonBlock(true)) < 0)
		{
			rNewSocket.closeSocket();
			return NULL;
		}

		addSocket(rNewSocket.getSocketFd());
		return &rNewSocket;
	}

	int CEpollModel::processWrite(int nSocketFd, char* pBuffer, int nBuffSize)
	{
		if (NULL == pBuffer)
		{
			return 0;
		}
		if (nSocketFd < 0 || nSocketFd >= mSocketCapacity)
		{
			return 0;
		}

		int nResult = mpAllSocket[nSocketFd].sendData(pBuffer, nBuffSize);
		if (nResult <= 0 || nResult != nBuffSize)
		{
			int nRemoveFd = mpAllSocket[nSocketFd].getSocketFd();
			mpAllSocket[nSocketFd].closeSocket();
			delSocket(nRemoveFd);
			return nResult;
		}
		return nResult;
	}
}
#endif //  MYTH_OS_UNIX

