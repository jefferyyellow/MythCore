#include "tcpsocket.h"

#ifdef MYTH_OS_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
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
		serverAddr.sin_addr.s_addr = inet_addr(pIP);
		serverAddr.sin_port = htons(uPort);
		return connect(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(sockaddr_in));
	}

	int CTcpSocket::reconnectServer()
	{
		closeSocket();
		sockaddr_in serverAddr;
		serverAddr.sin_addr.s_addr = inet_addr(mIP);
		serverAddr.sin_port = htons(mPort);
		return connect(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(sockaddr_in));
	}

	int	CTcpSocket::bindPort()
	{
		sockaddr_in serverAddr;
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = htons(mPort);
		return bind(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
	}

	int	CTcpSocket::bindAdress()
	{
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
		pNewSocket->setSocketFd(nFd);
		pNewSocket->setPort(ntohs(serverAddr.sin_port));
		pNewSocket->setIP(inet_ntoa(serverAddr.sin_addr));
		return 0;
	}

	void CTcpSocket::closeSocket()
	{
#ifdef MYTH_OS_WINDOWS
		closesocket(mSocketFd);
#else
		close(mSocketFd);
#endif
		setIP("");
		setPort(0);
	}

	int	CTcpSocket::setNonBlock(bool bBlock)
	{
#ifdef MYTH_OS_WINDOWS
		ULONG arg = bBlock ? 1 : 0;
		return ioctlsocket(mSocketFd, FIONBIO, &arg);
#else
		return 0;
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
}
