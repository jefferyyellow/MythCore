#ifndef __LOGINTYPE_H__
#define __LOGINTYPE_H__
#include "commontype.h"
enum EmTcpError
{
	emTcpError_None				= 0,	// δ֪
	emTcpError_OffLineClose		= 1,	// ���߹ر�
};

struct CExchangeHead
{
	time_t	mSocketTime;				// socket����ʱ��
	uint16	mSocketIndex;				// socket����
	uint16	mSocketError;				// socket����
};
#endif