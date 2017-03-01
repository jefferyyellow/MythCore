#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include"commontype.h"
#include "logmanager.h"
#include "logdisplayer.h"
#include "log.h"
#include "socketstream.h"
#include "sharememory.h"
#include "selectmodel.h"
#include "blockmemory.h"
#include "epollmodel.h"

#ifdef SMALL_SIZE
#define MAX_SOCKET_NUM				200				/*����TCPCtrl֧�ֵ�������*/
#endif

#ifdef MEDIUM_SIZE
#define MAX_SOCKET_NUM				3300				/*����TCPCtrl֧�ֵ�������*/
#endif

#ifdef LARGE_SIZE
#define MAX_SOCKET_NUM				13500				/*����TCPCtrl֧�ֵ�������*/
#endif	

#define PIPE_SIZE					((int)0x1000000)	/*�ڴ�ܵ��Ĵ�С*/
#define MAX_TCPBUFF_LEN				0x100000			/*tcp��д�����С*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket��������С
#define MIN_PACKET_SIZE				6					// ��С�İ���С
#define MAX_SEND_PACKAGE_ONCE		1000				/*һ�δ��ڴ�ܵ���ȡ���������ݰ�*/


using namespace Myth;

struct CTcpSocketBuff
{
	char	mData[MAX_SOCKET_BUFF_SIZE];
};

enum emTcpState
{
	emTcpState_None					= 0,	// δ֪
	emTcpState_Connect				= 1,	// ����
	emTcpState_Close				= 2,	// �ر�
};

struct CExchangeHead
{
	uint32	mTcpIndex;
	uint8	mTcpState;
};

class CTcpServer
{
public:
	CTcpServer();
	~CTcpServer();

public:
	/// ��ʼ��
	bool		init();
	/// ��ʼ��־
	bool		initLog();
	/// ��ʼ�������ڴ�
	bool		initShareMemory();
	/// ��ʼ��Socket
	bool		initSocket();

	/// ����
	void		run();
	/// ������Ϣ
	void		receiveMessage();
	// �������յ�һ��ǰ����Ϣ
	void		onReceiveMessage(CTcpSocket* pSocket, int nIndex);
	// ���Ϳͻ�����Ϣ
	void		sendMessage();

	/// ��ʼΪ�˳���׼��
	void		clear();
	/// �˳�
	void		exit();

private:
	CLog*					mDefaultLog;
	CShareMemory*			mShareMemory;
	CSocketStream*			mTcp2ServerMemory;
	CSocketStream*			mServer2TcpMemory;
#ifdef MYTH_OS_WINDOWS
	CSelectModel*			mSelectModel;
#else
	CEpollModel*			mEpollModel;
#endif
	CTcpSocket*				mTcpSocket;

	CBlockMemory<CTcpSocketBuff, 20, 20>	mSocketBuffPool;
	char					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];
	CExchangeHead			mExchangeHead;
};
#endif