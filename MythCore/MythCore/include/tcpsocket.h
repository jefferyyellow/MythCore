#ifndef __SOCKET_H__
#define __SOCKET_H__
#include "commontype.h"
#ifdef MYTH_OS_UNIX
#define SOCKET int
#endif
namespace Myth
{
	#define IP_SIZE			16 // 192.168.223.222����\0Ҳ��16����ipv6�Ժ���˵
	enum TmSocketStatus
	{
		emSocketStatusClose			= 0,		/// �ر�״̬
		emSocketStatusConnecting	= 1,		/// ������״̬
		emSocketStatusConnected		= 2,		/// ������״̬
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
		bool	IsNonBlock();

		int		getLinger();
		int		setLinger(int nTime);

		bool	checkReuseAddr();
		int		setReuseAddr(bool bReuse);

	public:
		uint32	getSocketFd(){ return mSocketFd; }
		void	setSocketFd(uint32 uFd){ mSocketFd = uFd; }

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
}
#endif
