#ifndef __LOGINTYPE_H__
#define __LOGINTYPE_H__
#include "commontype.h"
struct CExchangeHead
{
	uint32	mSocketTime;				// socket建立时间
	uint16	mSocketIndex;				// socket索引
	uint16	mSocketState;				// socket状态
};
#endif