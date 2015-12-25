#include "socket.h"
namespace Myth
{
#ifdef MYTH_OS_WINDOWS
	SOCKET CSocket::createSocket()
	{
		mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
		return mSocketFd;
	}

	int	CSocket::connectServer(char* pIP, uint16 uPort)
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

	int CSocket::reconnectServer()
	{
		closeSocket();
		sockaddr_in serverAddr;
		serverAddr.sin_addr.s_addr = inet_addr(mIP);
		serverAddr.sin_port = htons(mPort);
		return connect(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(sockaddr_in));
	}

	int	CSocket::bindPort()
	{
		sockaddr_in serverAddr;
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = htons(mPort);
		return bind(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
	}

	int	CSocket::bindAdress()
	{
		sockaddr_in serverAddr;
		serverAddr.sin_addr.s_addr = inet_addr(mIP);
		serverAddr.sin_port = htons(mPort);
		return bind(mSocketFd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
		return 0;
	}

	int	CSocket::checkNoBlockConnected(int nMS)
	{

		return 0;
	}

	int	CSocket::listenSocket(int nListNum)
	{
		return listen(mSocketFd, nListNum);
	}

	int	CSocket::acceptConnection(CSocket* pNewSocket)
	{
		sockaddr_in serverAddr;
		int nLen = sizeof(mIP);
		SOCKET  nFd = accept(mSocketFd, (struct sockaddr*)&serverAddr, &nLen);
		pNewSocket->setSocketFd(nFd);
		pNewSocket->setPort(ntohs(serverAddr.sin_port));
		pNewSocket->setIP(inet_ntoa(serverAddr.sin_addr));
		return 0;
	}

	void CSocket::closeSocket()
	{
		closesocket(mSocketFd);
		setIP("");
		setPort(0);
	}

	int	CSocket::setNonBlock(bool bBlock)
	{
		ULONG arg = bBlock ? 1 : 0;
		return ioctlsocket(mSocketFd, FIONBIO, &arg);
	}


	int CSocket::getLinger()
	{
		struct linger ling;
		int nLen = sizeof(ling);
		getsockopt(mSocketFd, SOL_SOCKET, SO_LINGER, (char*)&ling, &nLen);
		return ling.l_linger;
	}

	int CSocket::setLinger(int nTime)
	{
		struct linger ling;
		ling.l_onoff = nTime > 0 ? 1 : 0;
		ling.l_linger = nTime;

		return setsockopt(mSocketFd, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
	}

	bool CSocket::checkReuseAddr()
	{
		int nReuse = 0;
		int nLen = sizeof(nReuse);
		getsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, (char*)&nReuse, &nLen);
		return nReuse == 1;
	}

	int CSocket::setReuseAddr(bool bReuse)
	{
		UINT opt = (bReuse ? 1 : 0);
		return setsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

	}

#else
	int	CSocket::connectServer(char* pIP, uint32 uPort)
	{
		setIP(pIP);
		setPort(uPort);
		return 0;
	}

	int	CSocket::bindPort()
	{
		return 0;
	}

	int	CSocket::bind()
	{
		return 0;
	}

	int	CSocket::checkNoBlockConnected(int nMS)
	{
		return 0;
	}

	int	CSocket::listen(int nListNum)
	{
		return 0;
	}

	int	CSocket::accept(CSocket* pNewSocket)
	{
		return 0;
	}
#endif // MYTH_OS_WINDOWS
}
