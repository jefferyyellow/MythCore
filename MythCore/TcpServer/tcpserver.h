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

enum emTcpState
{
	emTcpState_None					= 0,	// 未知
	emTcpState_Connect				= 1,	// 连接
	emTcpState_Close				= 2,	// 关闭
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