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
#define MAX_TCPBUFF_LEN				0x100000			/*tcp读写缓存大小*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket缓冲区大小
#define MIN_PACKET_SIZE				6					// 最小的包大小
#define MAX_SEND_PACKAGE_ONCE		1000				/*一次从内存管道中取的最大的数据包*/


using namespace Myth;

struct CTcpSocketBuff
{
	char	mData[MAX_SOCKET_BUFF_SIZE];
};

enum EmTcpError
{
	emTcpError_None					= 0,	// 未知
	// 1-15表示从游戏服务器到TCP服务器的TCP错误状态通知，
	// 这些状态由游戏服务器设置
	emTcpError_OffLineClose			= 1,	// 下线关闭


	// 16-31表示从TCP服务器到游戏服务器的TCP错误状态通知，
	// 这些状态由TCP服务器设置
	emTcpError_SendData				= 16,	// 发送数据错误
};

struct CExchangeHead
{
	time_t	mSocketTime;				// socket建立时间
	uint16	mSocketIndex;				// socket索引
	uint16	mSocketError;				// socket错误
};

struct  CSocketInfo
{
	time_t	mCreateTime;			// socket的创建时间
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
	/// 初始化共享内存
	bool		initShareMemory();
	/// 初始化Socket
	bool		initSocket();

	/// 运行
	void		run();
	/// 接收消息
	void		receiveMessage();
	// 服务器收到一个前端消息
	void		onReceiveMessage(CTcpSocket* pSocket, int nIndex);
	// 发送客户端消息
	void		sendMessage();

	/// 开始为退出做准备
	void		clear();
	/// 退出
	void		exit();

	// 通知游戏服务器删除一个socket
	void		sendSocketErrToGameServer(int nTcpIndex, uint16 nSocketState);
	// 清除socket info
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