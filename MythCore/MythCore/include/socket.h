#ifndef __SOCKET_H__
#define __SOCKET_H__
#include "commontype.h"
namespace Myth
{
	#define IP_SIZE			16 // 192.168.223.222加上\0也就16个，ipv6以后再说
	enum TmSocketStatus
	{
		emSocketStatusClose			= 0,		/// 关闭状态
		emSocketStatusConnecting	= 1,		/// 连接中状态
		emSocketStatusConnected		= 2,		/// 已连接状态
	};
#ifdef MYTH_OS_WINDOWS
	class CSocket
	{
		CSocket()
		{
			mSocketFd = 0;
			mIP[0] = '\0';
			mPort = 0;
			mSocketStatus = emSocketStatusClose;
		}
		CSocket(char* pIP, uint32 uPort)
		{
			if (NULL != pIP)
			{
				setIP(pIP);
			}
			setPort(uPort);
		}
		~CSocket()
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
		int		acceptConnection(CSocket* pNewSocket);
		void	closeSocket();
		int		setNonBlock(bool bBlock);
		bool	IsNonBlock();

		int		getLinger();
		int		setLinger(int nTime);

		bool	checkReuseAddr();
		int		setReuseAddr(bool bReuse);

	public:
		uint32	getSocketFd(){ return mSocketFd; }
		void	setSocketFd(uint32 uFd){ mSocketFd = uFd; }

		const char*	getIP(){ return mIP; }
		void	setIP(char* strIP)
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

	private:
		/// socket file des
		SOCKET	mSocketFd;
		/// IP adress
		char	mIP[IP_SIZE];
		/// IP port
		uint16	mPort;
		/// socket status
		uint16	mSocketStatus;
	};
#else
	class CSocket
	{
		CSocket()
		{
			mSocketFd = 0;
			mIP[0] = '\0';
			mPort = 0;
			mSocketStatus = emSocketStatusClose;
		}
		CSocket(char* pIP, uint32 uPort)
		{
			if (NULL != pIP)
			{
				setIP(pIP);
			}
			setPort(uPort);
		}
		~CSocket()
		{

		}

	public:
		int		connectServer(char* pIP, uint32 uPort);
		int		bindPort();
		int		bindAdress();
		int		checkNoBlockConnected(int nMS);
		int		listenSocket(int nListNum);
		int		acceptConnection(CSocket* pNewSocket);

	public:
		uint32	getSocketFd(){ return mSocketFd; }
		void	setSocketFd(uint32 uFd){ mSocketFd = uFd; }

		const char*	getIP(){ return mIP; }
		void	setIP(char* strIP)
		{
			if (NULL == strIP)
			{
				return;
			}
			strncpy(mIP, strIP, sizeof(mIP) - 1);
			mIP[sizeof(mIP) - 1] = '\0';
		}

		uint32	getPort(){ return mPort; }
		void	setPort(uint32 uPort){ mPort = uPort; }

	private:
		/// socket file des
		uint32	mSocketFd;
		/// IP adress
		char	mIP[IP_SIZE];
		/// IP port
		uint32	mPort;
		/// socket status
		uint32	mSocketStatus;
	};
#endif // MYTH_OS_WINDOWS
}
#endif