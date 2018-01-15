#ifndef __SOCKET_H__
#define __SOCKET_H__
#include "commontype.h"
#ifdef MYTH_OS_UNIX
#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif
namespace Myth
{
	#define IP_SIZE			16 // 192.168.223.222加上\0也就16个，ipv6以后再说
	enum EmSocketStatus
	{
		emSocketStatusClose			= 0,		/// 关闭状态
		emSocketStatusConnecting	= 1,		/// 连接中状态
		emSocketStatusConnected		= 2,		/// 已连接状态
	};
	/*class CTcpBuff
	{
	CTcpBuff()
	{
	mpBuff = NULL;
	mBuffSize = 0;
	}

	~CTcpBuff()
	{
	if (NULL != mpBuff)
	{
	delete mpBuff;
	mpBuff = NULL;
	}
	mBuffSize = 0;
	}

	public:
	int			CreateBuffer(int nBuffSize)
	{
	mpBuff = new char[nBuffSize];
	if (NULL == mpBuff)
	{
	return 0;
	}
	return mBuffSize;
	}

	char*		GetBuff(){ return mpBuff; }

	int			GetBuffSize(){return mBuffSize;}
	void		SetBuffSize(int nBuffSize){mBuffSize = nBuffSize;}

	private:
	char*		mpBuff;
	int			mBuffSize;
	};*/

	class CTcpSocket
	{
	public:
		CTcpSocket()
		{
			mSocketFd = INVALID_SOCKET;
			mIP[0] = '\0';
			mPort = 0;
			mSocketStatus = emSocketStatusClose;
			mbListen = 0;

			mpRecvBuff = NULL;
			mRecvBuffSize = 0;
			mMaxRecvBuffSize = 0;
		}
		CTcpSocket(char* pIP, short uPort)
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
		int		connectServer(char* pIP, short uPort);
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

		int		getSendBuffSizeOption();
		int		setSendBuffSizeOption(int nBuffSize);

		int		getRecvBuffSizeOption();
		int		setRecvBuffSizeOption(int nBuffSize);

		int		sendData(byte* pBuff, int nBuffSize);
		int		recvData(byte* pBuff, int nBuffSize);
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

		short	getPort(){ return mPort; }
		void	setPort(short uPort){ mPort = uPort; }

		byte	getSocketStatus(){return mSocketStatus;}
		void	SetSocketStatus(byte nStatus){mSocketStatus = nStatus;}

		byte	GetListen(){return mbListen;}
		void	SetListen(byte bListen){mbListen = bListen;}


		byte*		getRecvBuffPoint(){ return mpRecvBuff + mRecvBuffSize; }
		short		getRecvBuffCapacity(){ return mMaxRecvBuffSize - mRecvBuffSize; }

		byte*		getRecvBuff(){return mpRecvBuff;}
		void		setRecvBuff(byte* pRecvBuff){mpRecvBuff = pRecvBuff;}

		short		getRecvBuffSize(){return mRecvBuffSize;}
		void		setRecvBuffSize(short nRecvBuffSize){mRecvBuffSize = nRecvBuffSize;}

		void		setMaxRecvBuffSize(short nMaxRecvBuffSize){mMaxRecvBuffSize = nMaxRecvBuffSize;}

		void		resetRecvBuffPoint(int nSize)
		{

			mRecvBuffSize -= nSize;
			if (mRecvBuffSize <= 0)
			{
				mRecvBuffSize = 0;
				return;
			}
			memmove(mpRecvBuff, mpRecvBuff + nSize, mRecvBuffSize);
		}

	private:
		/// socket file des
		SOCKET	mSocketFd;
		/// IP adress
		char	mIP[IP_SIZE];
		/// IP port
		short	mPort;
		/// socket status
		byte	mSocketStatus;
		/// listen socket
		byte	mbListen;
		/// 接收缓存
		byte*	mpRecvBuff;
		/// 接受缓存已经接受的数据长度
		short	mRecvBuffSize;
		/// 接受缓存最大的数据长度
		short	mMaxRecvBuffSize;
	};

	//class CBuffTcpSocket : public CTcpSocket
	//{
	//public:
	//	CBuffTcpSocket()
	//	{
	//		mpSendBuff = NULL;
	//		mSendBuffSize = 0;
	//		mMaxSendBuffSize = 0;
	//		mpRecvBuff = NULL;
	//		mRecvBuffSize = 0;
	//		mMaxRecvBuffSize = 0;
	//	}
	//	~CBuffTcpSocket()
	//	{
	//		clear();
	//	}

	//public:
	//	int			createBuffer(int nRecvBuffSize, int nSendBuffSize)
	//	{
	//		if (nRecvBuffSize > 0)
	//		{
	//			mpRecvBuff = new char[nRecvBuffSize];
	//			if (NULL == mpRecvBuff)
	//			{
	//				clear();
	//				return -1;
	//			}
	//			mRecvBuffSize = 0;
	//			mMaxRecvBuffSize = nRecvBuffSize;
	//		}
	//		
	//		if (nSendBuffSize > 0)
	//		{
	//			mpSendBuff = new char[nSendBuffSize];
	//			if (NULL == mpSendBuff)
	//			{
	//				clear();
	//				return -1;
	//			}
	//			mSendBuffSize = 0;
	//			mMaxSendBuffSize = nSendBuffSize;
	//		}

	//		return 0;
	//	}

	//	void		clear()
	//	{
	//		if (NULL != mpRecvBuff)
	//		{
	//			delete[]mpRecvBuff;
	//			mpRecvBuff = NULL;
	//		}

	//		if (NULL != mpSendBuff)
	//		{
	//			delete[]mpSendBuff;
	//			mpSendBuff = NULL;
	//		}

	//		mSendBuffSize = 0;
	//		mMaxSendBuffSize = 0;
	//		mRecvBuffSize = 0;
	//		mMaxRecvBuffSize = 0;
	//	}

	//	void		resetSendBuffPoint(int nSize)
	//	{
	//		if (mSendBuffSize <= 0 || nSize <= 0)
	//		{
	//			return;
	//		}
	//		memmove(mpSendBuff, mpSendBuff + nSize, mSendBuffSize);
	//	}

	//	void		resetRecvBuffPoint(int nSize)
	//	{
	//		if (mRecvBuffSize <= 0 || nSize <= 0)
	//		{
	//			return;
	//		}

	//		memmove(mpRecvBuff, mpRecvBuff + nSize, mRecvBuffSize);
	//	}

	//	char*		getRecvBuffPoint(){return mpRecvBuff + mRecvBuffSize;}
	//	int			getRecvBuffCapacity(){return mMaxRecvBuffSize - mRecvBuffSize;}

	//	char*		getSendBuff(){return mpSendBuff;}
	//	int			getSendBuffSize(){return mSendBuffSize;}
	//	void		setSendBuffSize(int nSendBuffSize){mSendBuffSize = nSendBuffSize;}

	//	char*		getRecvBuff(){return mpRecvBuff;}
	//	int			getRecvBuffSize(){return mRecvBuffSize;}
	//	void		setRecvBuffSize(int nRecvBuffSize){mRecvBuffSize = nRecvBuffSize;}

	//private:
	//	char*		mpSendBuff;
	//	int			mSendBuffSize;
	//	int			mMaxSendBuffSize;
	//	char*		mpRecvBuff;
	//	int			mRecvBuffSize;
	//	int			mMaxRecvBuffSize;
	//};
}
#endif
