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
#define SOCKET_CACHE_SIZE			102400				// socket�Ļ���
#define MAX_SOCKET_BUFF_SIZE		40960				// Socket���ݻ�������С
#define MIN_PACKET_SIZE				6					// ��С�İ���С
#define MAX_SEND_PACKAGE_ONCE		1000				/*һ�δ��ڴ�ܵ���ȡ���������ݰ�*/
#define	MAX_LISTEN_PORT_NUM			2					// ���������˿���Ŀ
#define MAX_GATE_BUFF_SIZE			10*1024*1024		// 10M
#define MAX_IP_CHAR_SIZE			20
using namespace Myth;

enum EmTcpError
{
	emTcpError_None					= 0,	// δ֪
	// 1-15��ʾ����Ϸ��������TCP��������TCP����״̬֪ͨ��
	// ��Щ״̬����Ϸ����������
	emTcpError_OffLineClose			= 1,	// ���߹ر�


	// 16-31��ʾ��TCP����������Ϸ��������TCP����״̬֪ͨ��
	// ��Щ״̬��TCP����������
	emTcpError_SendData				= 16,	// �������ݴ���
	emTcpError_ReadData				= 17,	// ��ȡ���ݴ���
	emTcpError_KeepLive				= 18,	// keep live������keep live��ʱ����û�н��յ�����
};

struct CTcpConfig
{
public:
	CTcpConfig()
	{
		memset(mListenPort, 0, sizeof(mListenPort));
		mKeepLiveTime = 300;
		mCheckLiveTime = 5;
		mWriteStatisticsTime = 60;
		mGameSvrIP[0] = '\0';
		mGameSvrPort = 0;
	}
	~CTcpConfig()
	{

	}
public:
	int		mListenPort[MAX_LISTEN_PORT_NUM];		// ���������˿���Ŀ
	int		mKeepLiveTime;							// ���ֻ��ŵ�ʱ��
	int		mCheckLiveTime;							// �����ŵ�ʱ����
	int		mWriteStatisticsTime;					// д��ͳ�����ݵ�ʱ��
	char	mGameSvrIP[MAX_IP_CHAR_SIZE];			// ��Ϸ������IP
	short	mGameSvrPort;							// ��Ϸ�������˿�
};

struct CTcpServerStatistics
{
public:
	CTcpServerStatistics()
	{
		mConnects = 0;
		mTotalConnects = 0;
		mReceiveBytes = 0;
		mReceiveMessageNum = 0;
		mSendBytes = 0;
		mSendMessageNum = 0;
	}
	~CTcpServerStatistics()
	{

	}
public:
	int		mConnects;								// ÿ��ʱ�������������������
	int		mTotalConnects;							// �ܹ���������
	int		mReceiveBytes;							// ÿ��ʱ���������յ����ֽ���
	int		mReceiveMessageNum;						// ÿ��ʱ���������յ�����Ϣ��Ŀ
	int		mSendBytes;								// ÿ��ʱ�������ڷ��͵��ֽ���
	int		mSendMessageNum;						// ÿ��ʱ�������ڷ��͵���Ϣ��Ŀ
};

struct CExchangeHead
{
	short	mDataLength;				// ���ݳ���
	time_t	mSocketTime;				// socket����ʱ��
	short	mSocketIndex;				// socket����
	short	mSocketError;				// socket����
};

struct  CSocketInfo
{
	time_t	mCreateTime;			// socket�Ĵ���ʱ��
	time_t	mKeepLiveTime;			// ���ֻ�Ծʱ��
	
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
	/// ��ʼ��Socket
	bool		initSocket();
	/// ��ʼ����Ϊһ���ͻ��˵�socket
	bool		initGameSvrSocket();

	/// ����
	void		run();
	/// ����Ƿ�ʱ
	void		checkTimeOut();
	/// ���keep live �Ƿ�ʱ
	void		checkKeepLiveTimeOut(time_t tTimeNow);
	/// ������Ϣ
	void		receiveClientMessage();
	/// �������յ�ǰ����Ϣ
	void		onReceiveMessage(CTcpSocket* pSocket, int nIndex);

	/// ������Ϸ��������Ϣ
	void		receiveGameServerMsg();
	/// ������Ϸ����������
	void		onReceiveGameServerMsg(CTcpSocket* pSocket);

	/// ��ʼΪ�˳���׼��
	void		clear();
	/// �˳�
	void		exit();

	/// ֪ͨ��Ϸ������ɾ��һ��socket
	void		sendSocketErrToGameServer(int nTcpIndex, short nSocketState);
	/// ���socket info
	void		clearSocketInfo(int nTcpIndex, CTcpSocket* pSocket);
	/// ����TCP����������
	void		loadTcpServerConfig(const char* pConfigPath);
	/// дͳ����Ϣ
	void		writeTcpStatisticsData();
	/// �����־
	void		clearLog(CLog* pLog);
private:
	CLog*					mDefaultLog;				// Ĭ����־
	CLog*					mStatisticsLog;				// ͳ����Ϣ��־
#ifdef MYTH_OS_WINDOWS
	CSelectModel*			mSelectModel;				// Selectģ��
#else
	CEpollModel*			mEpollModel;				// Epollģ��
#endif
	CTcpSocket*				mTcpSocket;					// Socket
	CSocketInfo*			mSocketInfo;				// Socket����Ϣ

	byte					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];	// ��Ϣ����
	CExchangeHead			mExchangeHead;				// TCP����Ϸ����������Ϣ����ͷ
	CTcpConfig				mTcpConfig;					// ����
	time_t					mLastKeepLiveTime;			// ��鱣�ֻ�Ծ��ʱ��
	time_t					mLastStatisticsTime;		// дͳ����Ϣʱ��
	CTcpServerStatistics	mServerStatistics;			// ͳ����Ϣ
	time_t					mLastTime;
	time_t					mCurrTime;					// ��ǰʱ��

	// ������Ϸ������(Tcp��һ���ͻ��˷�ʽ����)
	CTcpSocket				mClientSocket;				// ������Ϸ��������socket
	CSelectModel			mClientModel;				// ������Ϸ��������selectģ��
	byte*					mpClientRecData;			// ���ջ�����
};
#endif