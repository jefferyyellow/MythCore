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
	short	mSocketIndex;				// socket索引
	short	mSocketError;				// socket错误
};


#define MAX_PLAYER_NAME_LEN		32		// 玩家名字最大长度

enum EmPlayerStatus
{
	emPlayerStatus_None			= 0,	// 无状态
	emPlayerStatus_Loading		= 1,	// 加载状态
	emPlayerStatus_Gameing		= 2,	// 游戏状态
	emPlayerStatus_Exiting		= 3,	// 退出状态
};
#endif