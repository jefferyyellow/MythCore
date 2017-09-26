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
	// �ر�socket
	void ShutDownSocket(int nFd);
	// ����ָ���˿�
	int Listen(int nPort);
	// ���ɷ������׽���
	int SocketNoBlock(int nAF, int nType, int nProtocol);
	// ÿ֡�ص��ĺ���
	void OnFrame();
	// 
	void ReadInput();
	// ��������
	int ReceiveData(int nFd);
	// ���socket
	void ClearSocket(int nFd, int nError);
	int TcpRead(SOCKET nFd, BYTE *pBuf, int nLen);
	int TcpWrite(SOCKET nFd, BYTE *pBuf, int nLen);


	int OnMessage(CMessage* pMessage, int nFd);
	// ֹͣ������
	void StopServer(int nServerNum);
	// ����������
	void RestartServer(int nFd,int nServerNum);

	// �����׽����ַ���
	int CreateSocketStream(BYTE* pBuffer);

	// �������������ܱ��̳�
	int SetFdNotInherited(int nFd);

	// ���������ڴ�
//	int	CreateShareMemory();
	// �������ʱ��
	void CheckServerRestart();
public:
//	CShareMemory*			mC2SShareMemory;		// �ͻ���->�����������ڴ�
//	CShareMemory*			mS2CShareMemory;		// ������->�ͻ��˹����ڴ�
//
//	CSocketStream*			mC2SSocketStream;		// �ͻ���->�������׽�����
//	CSocketStream*			mS2CSocketStream;		// ������->�ͻ����׽�����
	
	// ����Ϊ������Ϣ
	int						mMaxFd;							// ���ľ��
	int						mEpollFd;						// Epoll�ľ��
	struct epoll_event*		mpEpollEvent;					// Epoll���¼�
	struct epoll_event		mctlEvent;						// Epoll�Ŀ����¼�
	CTcpSocket*				mpTcpSocket[MAX_SOCKET_NUM];	// ���е�socket
	CConfigFile				mConfigFile;
	int						mEpollEventFd;							// Epoll �������ļ�������
	// ʱ��
	int						mTimeZone;
	int						mTimeSecond;

	//��ʱ
	CRestartManager         mRestartManager;                  //������������ʱ��
public:
	// ��ͻ��˷��ͷ��������ƵĻ�Ӧ
	void SendGetServerNameResponse(int nFd);
	// ��ͻ��˷��ͷ������������
	void SendServerRestartResponse(int nFd, int nServerNum, int nResult);
};
#endif
