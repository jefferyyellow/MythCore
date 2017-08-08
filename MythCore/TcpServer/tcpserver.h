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

enum EmTcpError
{
	emTcpError_None					= 0,	// δ֪
	// 1-15��ʾ����Ϸ��������TCP��������TCP����״̬֪ͨ��
	// ��Щ״̬����Ϸ����������
	emTcpError_OffLineClose			= 1,	// ���߹ر�


	// 16-31��ʾ��TCP����������Ϸ��������TCP����״̬֪ͨ��
	// ��Щ״̬��TCP����������
	emTcpError_SendData				= 16,	// �������ݴ���
};

struct CExchangeHead
{
	time_t	mSocketTime;				// socket����ʱ��
	uint16	mSocketIndex;				// socket����
	uint16	mSocketError;				// socket����
};

struct  CSocketInfo
{
	time_t	mCreateTime;			// socket�Ĵ���ʱ��
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

	// ֪ͨ��Ϸ������ɾ��һ��socket
	void		sendSocketErrToGameServer(int nTcpIndex, uint16 nSocketState);
	// ���socket info
	void		clearSocketInfo(int nTcpIndex);

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
	CSocketInfo*			mSocketInfo;

	CBlockMemory<CTcpSocketBuff, 20, 20>	mSocketBuffPool;
	char					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];
	CExchangeHead			mExchangeHead;
};
#endif