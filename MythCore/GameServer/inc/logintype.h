#ifndef __LOGINTYPE_H__
#define __LOGINTYPE_H__
#include "commontype.h"
enum EmTcpError
{
	emTcpError_None				= 0,	// 未知
	emTcpError_OffLineClose		= 1,	// 下线关闭
};

struct CExchangeHead
{
	time_t	mSocketTime;				// socket建立时间
	uint16	mSocketIndex;				// socket索引
	uint16	mSocketError;				// socket错误
};
#endif