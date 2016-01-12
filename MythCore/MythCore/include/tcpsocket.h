#ifndef __SOCKET_H__
#define __SOCKET_H__
#include "commontype.h"
#ifdef MYTH_OS_UNIX
#define SOCKET int
define SOCKET_ERROR -1
#endif
namespace Myth
{
	#define IP_SIZE			16 // 192.168.223.222加上\0也就16个，ipv6以后再说
	enum TmSocketStatus
	{
		emSocketStatusClose			= 0,		/// 关闭状态
		emSocketStatusConnecting	= 1,		/// 连接中状态
		emSocketStatusConnected		= 2,		/// 已连接状态
	};

	class CTcpSocket
	{
		CTcpSocket()
		{
			mSocketFd = 0;
			mIP[0] = '\0';
			mPort = 0;
			mSocketStatus = emSocketStatusClose;
		}
		CTcpSocket(char* pIP, uint32 uPort)
		{
			if (NULL != pIP)
			{
				setIP(pIP);
			}
			setPort(uPort);
		}
		~CTcpSocket()
		{

		}

	public:
		SOCKET	createSocket();
		int		connectServer(char* pIP, uint16 uPort);
		int		reconnectServer();
		int		bindPort();
		int		bindAdress();
		int		checkNoBlockConnected(int nMS);
		int		listenSocket(int nListNum);
		int		acceptConnection(CTcpSocket* pNewSocket);
		void	closeSocket();
	
		int		setNonBlock(bool bBlock);
		bool	getNonBlock();

		int		getLinger();
		int		setLinger(int nTime);

		bool	checkReuseAddr();
		int		setReuseAddr(bool bReuse);

		bool	checkKeepAlive();
		int		setKeepAlive(bool bKeepAlive);

		int		getSendBuffSize();
		int		setSendBuffSize(int nBuffSize);

		int		getRecvBuffSize();
		int		setRecvBuffSize(int nBuffSize);

		int		sendData(char* pBuff, int nBuffSize);
		int		recvData(char* pBuff, int nBuffSize);
	public:
		SOCKET	getSocketFd(){ return mSocketFd; }
		void	setSocketFd(SOCKET uFd){ mSocketFd = uFd; }

		const char*	getIP(){ return mIP; }
		void	setIP(const char* strIP)
		{
			if (NULL == strIP)
			{
				return;
			}
			strncpy(mIP, strIP, sizeof(mIP) - 1);
			mIP[sizeof(mIP) - 1] = '\0';
		}

		uint32	getPort(){ return mPort; }
		void	setPort(uint16 uPort){ mPort = uPort; }

		uint8	getSocketStatus(){return mSocketStatus;}
		void	SetSocketStatus(uint8 nStatus){mSocketStatus = nStatus;}

		uint8	GetListen(){return mbListen;}
		void	SetListen(uint8 bListen){mbListen = bListen;}

	private:
		/// socket file des
		SOCKET	mSocketFd;
		/// IP adress
		char	mIP[IP_SIZE];
		/// IP port
		uint16	mPort;
		/// socket status
		uint8	mSocketStatus;
		/// listen socket
		uint8	mbListen;
	};
}
#endif
