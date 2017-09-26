#ifndef __NETWORKDEFINE_H__
#define __NETWORKDEFINE_H__

#define RECV_BUF_LEN		(8 * 1024)			// ���տͻ�����Ϣ�Ļ�����
#define PIPE_SIZE			((int)0x1000000)	// �ڴ�ܵ��Ĵ�С

#include "BaseDefine.h"
enum TmSocketType
{
	emSocketType_Listen,				// ����socket
	emSocketType_Link,					// ����socket
	emSocketTypeNum
};

// ��װsocket
class CTcpSocket
{
public:
	int					mFd;						// socket���
	int					mRecvBytes;					// ���յ��ֽ���
	BYTE				mMsgBuffer[RECV_BUF_LEN];	// ��Ϣ������
	BYTE				mSocketType;				// socket����
};

#endif
