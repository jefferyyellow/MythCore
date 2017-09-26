#ifndef __NETWORKDEFINE_H__
#define __NETWORKDEFINE_H__

#define RECV_BUF_LEN		(8 * 1024)			// 接收客户端信息的缓冲区
#define PIPE_SIZE			((int)0x1000000)	// 内存管道的大小

#include "BaseDefine.h"
enum TmSocketType
{
	emSocketType_Listen,				// 侦听socket
	emSocketType_Link,					// 连接socket
	emSocketTypeNum
};

// 包装socket
class CTcpSocket
{
public:
	int					mFd;						// socket句柄
	int					mRecvBytes;					// 接收的字节数
	BYTE				mMsgBuffer[RECV_BUF_LEN];	// 消息缓冲区
	BYTE				mSocketType;				// socket类型
};

#endif
