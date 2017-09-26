#ifndef __EPOLLTCP_H__
#define __EPOLLTCP_H__

#define MAX_SOCKET_NUM 3000
#define MAX_STR_NUM 256

typedef int				SOCKET;
#include<sys/epoll.h>
#include "BaseDefine.h"
#include "message.h"
#include "config.h"
#include "ServerRestartTimer.h"

class CTcpSocket;

class CEpollTcp
{
public:
	CEpollTcp(void);
	~CEpollTcp(void);

public:
	int Init();
	int InitEpoll();
	int AddSocket(int nfd);
	int DelSocket(int nfd);
	int Close(int nfd);
	int ClearUp();
	// 关闭socket
	void ShutDownSocket(int nFd);
	// 侦听指定端口
	int Listen(int nPort);
	// 生成非阻塞套接字
	int SocketNoBlock(int nAF, int nType, int nProtocol);
	// 每帧必调的函数
	void OnFrame();
	// 
	void ReadInput();
	// 接收数据
	int ReceiveData(int nFd);
	// 清除socket
	void ClearSocket(int nFd, int nError);
	int TcpRead(SOCKET nFd, BYTE *pBuf, int nLen);
	int TcpWrite(SOCKET nFd, BYTE *pBuf, int nLen);


	int OnMessage(CMessage* pMessage, int nFd);
	// 停止服务器
	void StopServer(int nServerNum);
	// 重启服务器
	void RestartServer(int nFd,int nServerNum);

	// 创建套接字字符流
	int CreateSocketStream(BYTE* pBuffer);

	// 设置描述符不能被继承
	int SetFdNotInherited(int nFd);

	// 创建共享内存
//	int	CreateShareMemory();
	// 检测重启时间
	void CheckServerRestart();
public:
//	CShareMemory*			mC2SShareMemory;		// 客户端->服务器共享内存
//	CShareMemory*			mS2CShareMemory;		// 服务器->客户端共享内存
//
//	CSocketStream*			mC2SSocketStream;		// 客户端->服务器套接字流
//	CSocketStream*			mS2CSocketStream;		// 服务器->客户端套接字流
	
	// 下面为控制信息
	int						mMaxFd;							// 最大的句柄
	int						mEpollFd;						// Epoll的句柄
	struct epoll_event*		mpEpollEvent;					// Epoll的事件
	struct epoll_event		mctlEvent;						// Epoll的控制事件
	CTcpSocket*				mpTcpSocket[MAX_SOCKET_NUM];	// 所有的socket
	CConfigFile				mConfigFile;
	int						mEpollEventFd;							// Epoll 触发的文件描述符
	// 时区
	int						mTimeZone;
	int						mTimeSecond;

	//计时
	CRestartManager         mRestartManager;                  //服务器重启计时器
public:
	// 向客户端发送服务器名称的回应
	void SendGetServerNameResponse(int nFd);
	// 向客户端发送服务器重启情况
	void SendServerRestartResponse(int nFd, int nServerNum, int nResult);
};
#endif
