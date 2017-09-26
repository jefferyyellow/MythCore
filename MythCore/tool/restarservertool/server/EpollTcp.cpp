#include "EpollTcp.h"
#include <stddef.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include "NetWorkDefine.h"
#include <time.h>
#include <sys/time.h>

//#include "SocketStream.h"
//#include "ShareMemory.h"

CEpollTcp::CEpollTcp(void)
{
}

CEpollTcp::~CEpollTcp(void)
{
	mConfigFile.CloseFile();
}
//一天的秒数
#define SECONDS_OF_DAY   (60*60*24)
//一小时的秒数
#define SECONDS_OF_HOUR   (60*60)
int GetUTCAndLocalDiffSec()
{
	/*因为虚拟机存在问题,所以采用新方法得到时区差
	struct timezone tvTimeZone;
	gettimeofday(NULL, &tvTimeZone);

	//将西区时间转化为东区，time()得到的时间加上此时间即可    
	return (-tvTimeZone.tz_minuteswest*60)	;
	*/
	struct tm *pTempTm = NULL;
	time_t tSecondDay = 0;

	pTempTm = localtime(&tSecondDay);
	if( !pTempTm )
	{
		return 0;
	}

	int iDiffSeconds = pTempTm->tm_hour*SECONDS_OF_HOUR;
	// 西半球
	if (pTempTm->tm_yday != 0)
	{
		iDiffSeconds = iDiffSeconds - SECONDS_OF_DAY;
	}
	return iDiffSeconds;
}


int CEpollTcp::Init()
{
	for(int i = 0; i < MAX_SOCKET_NUM; ++ i)
	{
		mpTcpSocket[i] = new CTcpSocket;
		if(NULL == mpTcpSocket[i])
		{
			printf("malloc tcp socket error");
			return -1;
		}

		memset(mpTcpSocket[i], 0, sizeof(CTcpSocket));
	}

	mConfigFile.OpenFile("config.dat");
	// 本应该是除3600，除以36的目的是放大100倍，避免小数

	time_t tNow= time(NULL);
	tm* pTm= localtime(&tNow);
	int ndaylight = pTm->tm_isdst;
	printf("daylight savings time flag: %d\n", pTm->tm_isdst);

	printf("GetUTCAndLocalDiffSec() value: %d\n", GetUTCAndLocalDiffSec() / 36);

	mTimeZone = GetUTCAndLocalDiffSec() / 36;
	printf("mTimeZone value: %d\n", mTimeZone);
	mTimeSecond = mTimeZone * 36;
	mRestartManager.Init();
	return 0;
}

int CEpollTcp::InitEpoll()
{
	if(NULL != mpEpollEvent)
	{
		return -1;
	}
	mpEpollEvent = (struct epoll_event*)malloc(MAX_SOCKET_NUM * sizeof(struct epoll_event));
	if(NULL == mpEpollEvent)
	{
		printf("malloc epoll event error!");
		return -1;
	}

	mEpollFd = epoll_create(MAX_SOCKET_NUM);
	if(mEpollFd < 0)
	{
		printf("epoll create error");
		return -1;
	}
	SetFdNotInherited(mEpollFd);

	// 初始化控件事件
	memset((void*)&mctlEvent, 0, sizeof(struct epoll_event));
	mctlEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP;
	mctlEvent.data.ptr = NULL;
	mctlEvent.data.fd = -1;

	return 0;
}


int CEpollTcp::AddSocket(int nfd)
{
	mctlEvent.data.fd = nfd;
	if(epoll_ctl(mEpollFd, EPOLL_CTL_ADD, nfd, &mctlEvent) < 0)
	{
		return -1;
	}
	if (nfd > mMaxFd)
	{
		mMaxFd = nfd;
	}
	return 0;
}

int CEpollTcp::DelSocket(int nfd)
{
	mctlEvent.data.fd = nfd;
	if(epoll_ctl(mEpollFd, EPOLL_CTL_DEL, nfd, &mctlEvent) < 0)
	{
		return -1;
	}
	return 0;
}

int CEpollTcp::Close(int nfd)
{
	close(nfd);
	return 0;
}


int CEpollTcp::ClearUp()
{
	free(mpEpollEvent);
	close(mEpollFd);
	return 0;
}

int CEpollTcp::Listen(int nPort)
{
	int flags = 1;
	socklen_t tOptValue = 0;
	int nOptLen = sizeof(int);

	struct linger ling= {0};
	struct sockaddr_in addr;

	memset((void*)&addr, 0, sizeof(struct sockaddr_in));

	int nfd = SocketNoBlock(AF_INET, SOCK_STREAM, 0);
	if(nfd == -1)
	{
		return -1;
	}

	setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags));
	setsockopt(nfd, SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags));
	setsockopt(nfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(nfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		close(nfd);
		return -1;
	}


	nOptLen = sizeof(socklen_t);
	tOptValue = 300000;

	if(setsockopt(nfd, SOL_SOCKET, SO_SNDBUF, (const void*)&tOptValue, nOptLen))
	{
		printf("set send buffer size to %d failed!", tOptValue);
		Close(nfd);
		return -1;
	}
	if(!getsockopt(nfd, SOL_SOCKET, SO_SNDBUF, (void*)&tOptValue, (socklen_t*)&nOptLen))
	{
		printf("set send buffer of socket is %d.", tOptValue);
	}

	if(-1 == listen(nfd, 1024))
	{
		printf("listen %d connection error!", 1024);
		Close(nfd);
		return -1;
	}

	if(-1 == AddSocket(nfd))
	{
		Close(nfd);
		return -1;
	}

	mpTcpSocket[nfd]->mFd = nfd;
	mpTcpSocket[nfd]->mSocketType = emSocketType_Listen;

	SetFdNotInherited(nfd);

	return 0;
}

int CEpollTcp::SocketNoBlock(int nAF, int nType, int nProtocol)
{
	int nfd = socket(nAF, nType, nProtocol);
	int flags = 1;
	if(nfd == -1)
	{
		return -1;
	}

	if(ioctl(nfd, FIONBIO, &flags) 
		&& ((flags = fcntl(nfd, F_GETFL, 0)) < 0
		|| fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0))
	{
		close(nfd);
		return -1;
	}
	return nfd;
}

// 关闭socket
void CEpollTcp::ShutDownSocket(int nFd)
{
	shutdown(nFd, 2);
	close(nFd);
}

// 每帧必调的函数
void CEpollTcp::OnFrame()
{

}

// 清除socket
void CEpollTcp::ClearSocket(int nFd, int nError)
{
	// 
	if (0 != nError )
	{

	}

	if (nFd > 0)
	{
		DelSocket(nFd);
		ShutDownSocket(nFd);
		if (nFd >= mMaxFd)
		{
			for (int i = mMaxFd - 1; i >= 0; i++)
			{
				mMaxFd = i;
				break;
			}
		}
	}

	mpTcpSocket[nFd]->mFd = -1;
	mpTcpSocket[nFd]->mRecvBytes = 0;
}

// 读取输入
void CEpollTcp::ReadInput()
{
	int nCount = epoll_wait(mEpollFd, mpEpollEvent, mMaxFd + 1, 10);
	if (-1 == nCount)
	{
		printf("epoll_wait return -1!");
	}
	struct sockaddr_in  tSockAddr; 
	socklen_t tSockLen = sizeof(sockaddr);

	struct epoll_event* pEvent = mpEpollEvent;
	mEpollEventFd = pEvent->data.fd;
	
	CTcpSocket* pTcpSocket = NULL;
	int nfd = -1;
	for (int i = 0; i < nCount; ++ i)
	{
		nfd = pEvent->data.fd;
		int flags = 1;
		if (nfd <= 0)
		{
			printf("event->data.fd < 0!, %d", nfd);
			continue;
		}
		pTcpSocket = mpTcpSocket[nfd];

		if(0 != (EPOLLERR & pEvent->events))
		{
			printf("pEvent->events error, error:%d, nfd:%d, event 0x%08x", nfd, errno, pEvent->events);
			// 清理代码
			ClearSocket(nfd, -1);
			continue;
		}

		if(0 == (EPOLLIN & pEvent->events))
		{
			printf("pEvent->events does not generate input event!");
			continue;
		}

		if (emSocketType_Listen == pTcpSocket->mSocketType)
		{
			SOCKET nNewFd = accept(nfd, (struct sockaddr*)&tSockAddr, (socklen_t*)&tSockLen);
			if (nNewFd <= 0)
			{
				printf("accept error: %d", nfd);
				continue;
			}

			if (MAX_SOCKET_NUM <= nNewFd)
			{
				ShutDownSocket(nNewFd);
				continue;
			}

			if (ioctl(nNewFd, FIONBIO, &flags)
				&&((flags = fcntl(nNewFd, F_GETFL, 0)) < 0 || fcntl(nNewFd, F_SETFL, flags | O_NONBLOCK) < 0))
			{
				printf("operate on socket %d error!", nNewFd);
				ShutDownSocket(nNewFd);
				continue;
			}
			SetFdNotInherited(nNewFd);
			int nResult = AddSocket(nNewFd);
			if (0 != nResult)
			{
				printf("add to epoll failed, socket: %d", nNewFd);
				ShutDownSocket(nNewFd);
				continue;
			}
			if (nNewFd > mMaxFd)
			{
				mMaxFd = nNewFd;
			}

			mpTcpSocket[nNewFd]->mSocketType = emSocketType_Link;
			mpTcpSocket[nNewFd]->mRecvBytes = 0;
		}
		else
		{
			ReceiveData(nfd);
		}
	}
}

int CEpollTcp::TcpRead(SOCKET nFd, BYTE *pBuf, int nLen)
{
	int nReceiveCount = 0;
	while (true)
	{
		nReceiveCount = read(nFd, pBuf, nLen);
		if (nReceiveCount > 0)
		{
			return nReceiveCount;
		}
		else
		{
			if (0  > nReceiveCount && EINTR == errno)
			{
				continue;
			}
			return nReceiveCount;
		}
	}

	return -1;
}

// 写入数据
int CEpollTcp::TcpWrite(SOCKET nFd, BYTE *pBuf, int nLen)
{
	int nSendCount = 0;
	while(true)
	{
		nSendCount = write(nFd, pBuf, nLen);
		if (nLen == nSendCount)
		{
			return nSendCount;
		}
		else 
		{
			if (0 >= nSendCount && EINTR == errno)
			{
				continue;	
			}
			return nSendCount;
		}
	}

	return -1;
}

// 接收数据
int CEpollTcp::ReceiveData(int nFd)
{
	CTcpSocket* pSocket = mpTcpSocket[nFd];
	int nDataLength = pSocket->mRecvBytes;
	int nReceiveBytes = TcpRead(nFd, pSocket->mMsgBuffer + nDataLength, RECV_BUF_LEN - nDataLength);
	if (nReceiveBytes <= 0)
	{
		ClearSocket(nFd, -1);
		return -1;
	}

	pSocket->mRecvBytes += nReceiveBytes;

	if(pSocket->mRecvBytes < sizeof(CMessage))
	{
		return 0;
	}
	
	while(true)
	{
		CMessage* pMessage = (CMessage*)pSocket->mMsgBuffer;
		int nPackageSize = pMessage->Size;
		if(pMessage->Size <= pSocket->mRecvBytes)
		{
			OnMessage(pMessage, nFd);	
		}
		else
		{
			break;
		}

		pSocket->mRecvBytes -= nPackageSize;	
		memmove(pSocket->mMsgBuffer, pSocket->mMsgBuffer+nPackageSize,pSocket->mRecvBytes); 
		if (pSocket->mRecvBytes < sizeof(CMessage))
		{
			break;
		}
	}

	return 0;	
}

void CEpollTcp::StopServer(int nServerNum)
{
	char szConfigSection[MAX_STR_NUM] = {0}; 
	snprintf(szConfigSection, sizeof(szConfigSection), "serverinfo_%d", nServerNum);

	char szUserName[MAX_STR_NUM] = {0};
	mConfigFile.GetItemValue(szConfigSection, "username", szUserName, sizeof(szUserName));

	char szRestartPath[MAX_STR_NUM] = {0};
	mConfigFile.GetItemValue(szConfigSection, "restartpath", szRestartPath, sizeof(szRestartPath));

	char szCommand[MAX_STR_NUM] = {0};
	snprintf(szCommand, sizeof(szCommand), "su %s <<XXX\ncd\nsource .bash_profile\ncd %s\n./stop.sh\nXXX", szUserName, szRestartPath);


	system(szCommand);
}

void CEpollTcp::RestartServer(int nFd,int nServerNum)
{
	char szConfigSection[MAX_STR_NUM] = {0}; 
	snprintf(szConfigSection, sizeof(szConfigSection), "serverinfo_%d", nServerNum);

	char szUserName[MAX_STR_NUM] = {0};
	mConfigFile.GetItemValue(szConfigSection, "username", szUserName, sizeof(szUserName));

	char szRestartPath[MAX_STR_NUM] = {0};
	mConfigFile.GetItemValue(szConfigSection, "restartpath", szRestartPath, sizeof(szRestartPath));

	char szCommand[MAX_STR_NUM] = {0};
	snprintf(szCommand, sizeof(szCommand), "su %s <<XXX\ncd\nsource .bash_profile\ncd %s\n./restart.sh\nXXX", szUserName, szRestartPath);
	system(szCommand);

	mRestartManager.AddServerRestart(nFd, nServerNum, time(NULL));
}

int CEpollTcp::OnMessage(CMessage* pMessage, int nFd)
{
	int nID = pMessage->ID;
	switch(nID)
	{
	case emC2SRequest_RestartServer:
		{
			CMessageRestartServerRequest* pRestartServerRequest = (CMessageRestartServerRequest*)pMessage;
			for (int i = 0; i < MAX_SERVER_NUM; ++i)
			{
				if (pRestartServerRequest->SelectServer[i])
				{
					RestartServer(nFd, i);
				}
			}

			break;
		}
	case emC2SRequest_GetServerDateTime:
		{

			struct timeval tv;
			gettimeofday(&tv, NULL);
			tm* pTime = localtime(&tv.tv_sec);
			int nTime = tv.tv_sec + mTimeSecond + pTime->tm_isdst * 3600;	
			CMessageGetServerDateTimeResponse tGetServerDateTimeResponse;
			tGetServerDateTimeResponse.ID = emS2CResponse_GetServerDateTime;
			tGetServerDateTimeResponse.Size = sizeof(CMessageGetServerDateTimeResponse);
			tGetServerDateTimeResponse.DateTime= nTime;
			TcpWrite(nFd, (BYTE*)&tGetServerDateTimeResponse, sizeof(CMessageGetServerDateTimeResponse));

			break;
		};
	case emC2SRequest_SetServerDateTime:
		{
			CMessageSetServerDateTimeRequest* pSetServerDateTimeRequest = (CMessageSetServerDateTimeRequest*)pMessage;
			time_t tTime = pSetServerDateTimeRequest->DateTime;
			
			for (int i = 0; i < MAX_SERVER_NUM; ++i)
			{
				if (pSetServerDateTimeRequest->SelectServer[i])
				{
					StopServer(i);
				}
			}

			struct timeval tv;
			tm* pTime = localtime(&tTime);
			tv.tv_sec = tTime - mTimeSecond - pTime->tm_isdst * 3600;
			tv.tv_usec = 0;
			settimeofday(&tv, NULL);

			for (int i = 0; i < MAX_SERVER_NUM; ++i)
			{
				if (pSetServerDateTimeRequest->SelectServer[i])
				{
					RestartServer(nFd, i);
				}
			}

			gettimeofday(&tv, NULL);
			int nTime = tv.tv_sec + mTimeSecond + pTime->tm_isdst * 3600;	
			CMessageSetServerDateTimeResponse tSetServerDateTimeResponse;
			tSetServerDateTimeResponse.ID = emS2CResponse_SetServerDateTime;
			tSetServerDateTimeResponse.Size = sizeof(CMessageSetServerDateTimeResponse);
			tSetServerDateTimeResponse.DateTime = nTime;
			TcpWrite(nFd, (BYTE*)&tSetServerDateTimeResponse, sizeof(CMessageSetServerDateTimeResponse));
			break;
		}
	case emC2SRequest_StopServer:
		{
			CMessageStopServerRequest* pStopServerRequest = (CMessageStopServerRequest*)pMessage;
	
			for (int i = 0; i < MAX_SERVER_NUM; ++i)
			{
				if (pStopServerRequest->SelectServer[i])
				{
					StopServer(i);
				}
			}

			CMessageStopServerResponse tStopServerResponse;
			tStopServerResponse.Result=0;
			tStopServerResponse.Size = sizeof(CMessageStopServerResponse);
			tStopServerResponse.ID = emS2CResponse_StopServer;
			TcpWrite(nFd, (BYTE*)&tStopServerResponse, sizeof(CMessageStopServerResponse));	

			break;
		}
	case emS2CNotify_CompileComplete:
		{			
			CMessageCompileCompleteNotify* tpCompileCompleteNotify = (CMessageCompileCompleteNotify*)pMessage;
			TcpWrite(nFd, (BYTE*)tpCompileCompleteNotify, sizeof(CMessageCompileCompleteNotify));	
			break;
		}
	case emC2SRequest_GetServerName:
		{
			SendGetServerNameResponse(nFd);
			break;
		}
	default:
		break;
	}
}
// 设置描述符不能被继承
int CEpollTcp::SetFdNotInherited(int nFd)
{
	int nFlags = fcntl(nFd, F_GETFD, 0);
	fcntl(nFd, F_SETFD, nFlags | FD_CLOEXEC);	
}

// 创建套接字字符流
int CEpollTcp::CreateSocketStream(BYTE* pBuffer)
{
	return 0;
}

// 向客户端发送服务器名称的回应
void CEpollTcp::SendGetServerNameResponse(int nFd)
{
	CMessageGetServerNameResponse tMsgResponse;
	tMsgResponse.ID = emS2CResponse_GetServerName;
	tMsgResponse.Size = sizeof(CMessageGetServerNameResponse);

	for (int i = 0; i < MAX_SERVER_NUM; ++i)
	{
		char szConfigSection[MAX_STR_NUM] = {0}; 
		snprintf(szConfigSection, sizeof(szConfigSection), "serverinfo_%d", i);

		char szUserName[MAX_STR_NUM] = {0};
		mConfigFile.GetItemValue(szConfigSection, "servername", szUserName, sizeof(szUserName));
		//printf("\nUserName : %s\n", szUserName);
		int tSvrNameLen = strlen(szUserName);
		if ( tSvrNameLen == 0 )
		{
			continue;
		}

		strncpy(tMsgResponse.mServerName[i], szUserName, MAX_STR_NUM - 1);
		tMsgResponse.mServerName[i][MAX_STR_NUM - 1] = '\0';
	}

	TcpWrite(nFd, (BYTE*)&tMsgResponse, sizeof(CMessageGetServerNameResponse));	
}
// 创建共享内存
//int	CEpollTcp::CreateShareMemory()
//{
//	BYTE* pC2SBuffer = CShareMemory::CreateShareMemory(22222222, PIPE_SIZE);
//	mC2SShareMemory = (CShareMemory*)pC2SBuffer;
//	pC2SBuffer += sizeof(CShareMemory);
//	mC2SSocketStream = (CSocketStream*)pC2SBuffer;
//	mC2SSocketStream->Initialize(pC2SBuffer + sizeof(CSocketStream), PIPE_SIZE - sizeof(CShareMemory) - sizeof(CSocketStream));
//
//
//	BYTE* pS2CBuffer = CShareMemory::CreateShareMemory(22222223, PIPE_SIZE);
//	mS2CShareMemory = (CShareMemory*)pS2CBuffer;
//	pS2CBuffer += sizeof(CShareMemory);
//	mS2CSocketStream = (CSocketStream*)pS2CBuffer;
//	mS2CSocketStream->Initialize(pS2CBuffer + sizeof(CShareMemory), PIPE_SIZE - sizeof(CShareMemory) - sizeof(CSocketStream) );
//	return 0;
//}

void CEpollTcp::CheckServerRestart()
{
	int nFd[MAX_SOCKET_NUM] = { 0 };
	int nServerNum[MAX_SOCKET_NUM] = { 0 };
	int nSize = 0;
	int nOutFd[MAX_SOCKET_NUM] = { 0 };
	int nOutServerNum[MAX_SOCKET_NUM] = { 0 };
	int nOutSize = 0;
	mRestartManager.CheckRestartTimer(nFd, nServerNum, nSize, nOutFd, nOutServerNum, nOutSize,mConfigFile);

	for (int i = 0; i < nSize; ++i)
	{
		SendServerRestartResponse(nFd[i], nServerNum[i], SUCCESS);
	}

	for (int i = 0; i < nOutSize; ++i)
	{
		SendServerRestartResponse(nOutFd[i], nOutServerNum[i], UNKNOW_ERROR);
	}
}

void CEpollTcp::SendServerRestartResponse(int nFd, int nServerNum,int nResult)
{
	CMessageRestartServerResponse tMsgNotify;
	tMsgNotify.ID = emS2CResponse_RestartServer;
	tMsgNotify.Size = sizeof(CMessageRestartServerResponse);

	char szConfigSection[MAX_STR_NUM] = { 0 };
	snprintf(szConfigSection, sizeof(szConfigSection), "serverinfo_%d", nServerNum);

	char szUserName[MAX_STR_NUM] = { 0 };
	mConfigFile.GetItemValue(szConfigSection, "servername", szUserName, sizeof(szUserName));

	strncpy(tMsgNotify.ServerName, szUserName, MAX_STR_NUM - 1);
	tMsgNotify.ServerName[MAX_STR_NUM - 1] = '\0';

	tMsgNotify.Result = nResult;

	TcpWrite(nFd, (BYTE*)&tMsgNotify, sizeof(CMessageRestartServerResponse));
}