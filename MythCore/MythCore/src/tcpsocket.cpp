#include "tcpsocket.h"

#ifdef MYTH_OS_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#endif

namespace Myth
{
	SOCKET CTcpSocket::createSocket()
	{
		mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
		return mSocketFd;
	}

	int	CTcpSocket::connectServer(char* pIP, uint16 uPort)
	{
		if (NULL == pIP)
		{
			return -1;
		}
		setIP(pIP);
		setPort(uPort);
		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr(pIP);
		serverAddr.sin_port = htons(uPort);
		return connect(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(sockaddr_in));
	}

	int CTcpSocket::reconnectServer()
	{
		closeSocket();
		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr(mIP);
		serverAddr.sin_port = htons(mPort);
		return connect(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(sockaddr_in));
	}

	int	CTcpSocket::bindPort()
	{
		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = htons(mPort);
		return bind(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
	}

	int	CTcpSocket::bindAdress()
	{
		if ('\0' == mIP[0])
		{
			return -1;
		}
		sockaddr_in serverAddr;
		serverAddr.sin_addr.s_addr = inet_addr(mIP);
		serverAddr.sin_port = htons(mPort);
		return bind(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
		return 0;
	}

	int	CTcpSocket::checkNoBlockConnected(int nMS)
	{

		return 0;
	}

	int	CTcpSocket::listenSocket(int nListNum)
	{
		return listen(mSocketFd, nListNum);
	}

	int	CTcpSocket::acceptConnection(CTcpSocket* pNewSocket)
	{
		sockaddr_in serverAddr;
#ifdef MYTH_OS_WINDOWS
		int nLen = sizeof(serverAddr);
#else
		socklen_t nLen = sizeof(serverAddr);
#endif
		SOCKET  nFd = accept(mSocketFd, (struct sockaddr*)&serverAddr, &nLen);
		if (NULL != pNewSocket)
		{
			pNewSocket->setSocketFd(nFd);
			pNewSocket->setPort(ntohs(serverAddr.sin_port));
			pNewSocket->setIP(inet_ntoa(serverAddr.sin_addr));
		}
		return 0;
	}

	void CTcpSocket::closeSocket()
	{
#ifdef MYTH_OS_WINDOWS
		closesocket(mSocketFd);
#else
		shutdown(mSocketFd, SHUT_RDWR);
		close(mSocketFd);
#endif
		setIP("");
		setPort(0);
		mSocketFd = INVALID_SOCKET;
	}

	int	CTcpSocket::setNonBlock(bool bBlock)
	{
#ifdef MYTH_OS_WINDOWS
		ULONG arg = bBlock ? 1 : 0;
		return ioctlsocket(mSocketFd, FIONBIO, &arg);
#else
		int nFlag = fcntl(mSocketFd, F_GETFL, 0);
		if (nFlag < 0)
		{
			// 出错
			return -1;
		}
		if (bBlock)
		{
			nFlag |= O_NONBLOCK;
		}
		else
		{
			nFlag &= ~O_NONBLOCK;
		}
		int nResult = fcntl(mSocketFd, F_SETFL, nFlag);
		if (nResult < 0)
		{
			// 出错
			return -1;
		}
		return 0;
#endif
	}

	bool CTcpSocket::getNonBlock()
	{
#ifdef MYTH_OS_WINDOWS
		return 0;
#else
		int nFlag = fcntl(mSocketFd, F_GETFL, 0);
		if (nFlag < 0)
		{
			// 出错
		}
		return nFlag | O_NONBLOCK;
#endif
	}

	int CTcpSocket::getLinger()
	{
		struct linger ling;
#ifdef MYTH_OS_WINDOWS
		int nLen = sizeof(ling);
#else
		socklen_t nLen = sizeof(ling);
#endif
		getsockopt(mSocketFd, SOL_SOCKET, SO_LINGER, (char*)&ling, &nLen);
		return ling.l_linger;
	}

	int CTcpSocket::setLinger(int nTime)
	{
		struct linger ling;
		ling.l_onoff = nTime > 0 ? 1 : 0;
		ling.l_linger = nTime;

		return setsockopt(mSocketFd, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
	}

	bool CTcpSocket::checkReuseAddr()
	{
		int nReuse = 0;
#ifdef MYTH_OS_WINDOWS
		int nLen = sizeof(nReuse);
#else
		socklen_t nLen = sizeof(nReuse);
#endif
		getsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, (char*)&nReuse, &nLen);
		return nReuse == 1;
	}

	int CTcpSocket::setReuseAddr(bool bReuse)
	{
		int opt = (bReuse ? 1 : 0);
		return setsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
	}

	bool CTcpSocket::checkKeepAlive()
	{
		int nKeepAlive = 0;
#ifdef MYTH_OS_WINDOWS
		int nLen = sizeof(nKeepAlive);
#else
		socklen_t nLen = sizeof(nKeepAlive);
#endif
		getsockopt(mSocketFd, SOL_SOCKET, SO_KEEPALIVE, (char*)&nKeepAlive, &nLen);
		return nKeepAlive == 1;

	}

	int	CTcpSocket::setKeepAlive(bool bKeepAlive)
	{
		int opt = (bKeepAlive ? 1 : 0);
		return setsockopt(mSocketFd, SOL_SOCKET, SO_KEEPALIVE, (char*)&opt, sizeof(opt));
	}

	int CTcpSocket::getSendBuffSizeOption()
	{
		int nBuffSize = 0;
#ifdef MYTH_OS_WINDOWS
		int nLen = sizeof(int);
#else
		socklen_t nLen = sizeof(int);
#endif
		getsockopt(mSocketFd, SOL_SOCKET, SO_SNDBUF, (char *)&nBuffSize, &nLen);
		return nBuffSize;
	}

	int	CTcpSocket::setSendBuffSizeOption(int nBuffSize)
	{
#ifdef MYTH_OS_WINDOWS
		int nLen = sizeof(int);
#else
		socklen_t nLen = sizeof(int);
#endif
		return setsockopt(mSocketFd, SOL_SOCKET, SO_SNDBUF, (char *)&nBuffSize, nLen);
	}

	int	CTcpSocket::getRecvBuffSizeOption()
	{
		int nBuffSize = 0;
#ifdef MYTH_OS_WINDOWS
		int nLen = sizeof(int);
#else
		socklen_t nLen = sizeof(int);
#endif
		getsockopt(mSocketFd, SOL_SOCKET, SO_RCVBUF, (char *)&nBuffSize, &nLen);
		return nBuffSize;
	}

	int	CTcpSocket::setRecvBuffSizeOption(int nBuffSize)
	{
#ifdef MYTH_OS_WINDOWS
		int nLen = sizeof(int);
#else
		socklen_t nLen = sizeof(int);
#endif
		return setsockopt(mSocketFd, SOL_SOCKET, SO_RCVBUF, (char *)&nBuffSize, nLen);
	}

#ifdef MYTH_OS_WINDOWS
	int	CTcpSocket::sendData(char* pBuff, int nBuffSize)
	{
		int nSendBytes = 0;
		int nLeftLen = nBuffSize;
		
		while (true)
		{
			nSendBytes = send(mSocketFd, pBuff, nLeftLen, 0);
			if (nSendBytes == nLeftLen)
			{
				return nSendBytes;
			}
			else
			{
				if (nSendBytes <= 0)
				{
					return nSendBytes;
				}
			}

		}
	}
#else

	int	CTcpSocket::sendData(char* pBuff, int nBuffSize)
	{
		int nSendBytes = 0;
		int nLeftLen = nBuffSize;

		while (1)
		{
			// 将数据写入套接字
			nSendBytes = write(mSocketFd, pBuff, nLeftLen);
			if (nSendBytes == nLeftLen)
			{
				return nSendBytes;
			}
			else
			{
				if (0 >= nSendBytes && EINTR == errno)
				{
					continue;
				}

				return nSendBytes;
			}
		}
	}
#endif

#ifdef MYTH_OS_WINDOWS
	int	CTcpSocket::recvData(char* pBuff, int nBuffSize)
	{
		int nResult = recv(mSocketFd, pBuff, nBuffSize, 0);
		return nResult;
	}
#else
	int	CTcpSocket::recvData(char* pBuff, int nBuffSize)
	{
		int nRecvBytes =0;

		while (1)
		{
			nRecvBytes = read(mSocketFd, pBuff, nBuffSize);

			if (0 < nRecvBytes)
			{
				return nRecvBytes;
			}
			else
			{
				if (0 > nRecvBytes && errno == EINTR)
				{
					continue;
				}
				return nRecvBytes;
			}
		}
		return -1;
	}
#endif
}
