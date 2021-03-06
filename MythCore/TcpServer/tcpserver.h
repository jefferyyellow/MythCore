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
#define MAX_SOCKET_NUM				200				/*单个TCPCtrl支持的连接数*/
#endif

#ifdef MEDIUM_SIZE
#define MAX_SOCKET_NUM				3300				/*单个TCPCtrl支持的连接数*/
#endif

#ifdef LARGE_SIZE
#define MAX_SOCKET_NUM				13500				/*单个TCPCtrl支持的连接数*/
#endif	

#define PIPE_SIZE					((int)0x1000000)	/*内存管道的大小*/
#define SOCKET_CACHE_SIZE			102400				// socket的缓存
#define MAX_SOCKET_BUFF_SIZE		40960				// Socket数据缓冲区大小
#define MIN_PACKET_SIZE				6					// 最小的包大小
#define MAX_SEND_PACKAGE_ONCE		1000				/*一次从内存管道中取的最大的数据包*/
#define	MAX_LISTEN_PORT_NUM			2					// 最大的侦听端口数目
#define MAX_GATE_BUFF_SIZE			10*1024*1024		// 10M
#define MAX_IP_CHAR_SIZE			20
using namespace Myth;

enum EmTcpError
{
	emTcpError_None					= 0,	// 未知
	// 1-15表示从游戏服务器到TCP服务器的TCP错误状态通知，
	// 这些状态由游戏服务器设置
	emTcpError_OffLineClose			= 1,	// 下线关闭


	// 16-31表示从TCP服务器到游戏服务器的TCP错误状态通知，
	// 这些状态由TCP服务器设置
	emTcpError_SendData				= 16,	// 发送数据错误
	emTcpError_ReadData				= 17,	// 读取数据错误
	emTcpError_KeepLive				= 18,	// keep live错误，在keep live的时间内没有接收到数据
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
	int		mListenPort[MAX_LISTEN_PORT_NUM];		// 最大的侦听端口数目
	int		mKeepLiveTime;							// 保持活着的时间
	int		mCheckLiveTime;							// 检查活着的时间间隔
	int		mWriteStatisticsTime;					// 写入统计数据的时间
	char	mGameSvrIP[MAX_IP_CHAR_SIZE];			// 游戏服务器IP
	short	mGameSvrPort;							// 游戏服务器端口
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
	int		mConnects;								// 每个时间周期内请求的连接数
	int		mTotalConnects;							// 总共的连接数
	int		mReceiveBytes;							// 每个时间周期内收到的字节数
	int		mReceiveMessageNum;						// 每个时间周期内收到的消息数目
	int		mSendBytes;								// 每个时间周期内发送的字节数
	int		mSendMessageNum;						// 每个时间周期内发送的消息数目
};

struct CExchangeHead
{
	short	mDataLength;				// 数据长度
	time_t	mSocketTime;				// socket建立时间
	short	mSocketIndex;				// socket索引
	short	mSocketError;				// socket错误
};

struct  CSocketInfo
{
	time_t	mCreateTime;			// socket的创建时间
	time_t	mKeepLiveTime;			// 保持活跃时间
	
};

class CTcpServer
{
public:
	CTcpServer();
	~CTcpServer();

public:
	/// 初始化
	bool		init();
	/// 初始日志
	bool		initLog();
	/// 初始化Socket
	bool		initSocket();
	/// 初始化作为一个客户端的socket
	bool		initGameSvrSocket();

	/// 运行
	void		run();
	/// 检查是否超时
	void		checkTimeOut();
	/// 检测keep live 是否超时
	void		checkKeepLiveTimeOut(time_t tTimeNow);
	/// 接收消息
	void		receiveClientMessage();
	/// 服务器收到前端消息
	void		onReceiveMessage(CTcpSocket* pSocket, int nIndex);

	/// 接收游戏服务器消息
	void		receiveGameServerMsg();
	/// 接收游戏服务器处理
	void		onReceiveGameServerMsg(CTcpSocket* pSocket);

	/// 开始为退出做准备
	void		clear();
	/// 退出
	void		exit();

	/// 通知游戏服务器删除一个socket
	void		sendSocketErrToGameServer(int nTcpIndex, short nSocketState);
	/// 清除socket info
	void		clearSocketInfo(int nTcpIndex, CTcpSocket* pSocket);
	/// 加载TCP服务器配置
	void		loadTcpServerConfig(const char* pConfigPath);
	/// 写统计信息
	void		writeTcpStatisticsData();
	/// 清除日志
	void		clearLog(CLog* pLog);
private:
	CLog*					mDefaultLog;				// 默认日志
	CLog*					mStatisticsLog;				// 统计信息日志
#ifdef MYTH_OS_WINDOWS
	CSelectModel*			mSelectModel;				// Select模型
#else
	CEpollModel*			mEpollModel;				// Epoll模型
#endif
	CTcpSocket*				mTcpSocket;					// Socket
	CSocketInfo*			mSocketInfo;				// Socket的信息

	byte					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];	// 消息缓存
	CExchangeHead			mExchangeHead;				// TCP和游戏服务器的消息交换头
	CTcpConfig				mTcpConfig;					// 配置
	time_t					mLastKeepLiveTime;			// 检查保持活跃的时间
	time_t					mLastStatisticsTime;		// 写统计信息时间
	CTcpServerStatistics	mServerStatistics;			// 统计信息
	time_t					mLastTime;
	time_t					mCurrTime;					// 当前时间

	// 连接游戏服务器(Tcp以一个客户端方式连接)
	CTcpSocket				mClientSocket;				// 连接游戏服务器的socket
	CSelectModel			mClientModel;				// 连接游戏服务器的select模型
	byte*					mpClientRecData;			// 接收缓冲区
};
#endif