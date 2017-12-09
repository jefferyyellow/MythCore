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
	short	mSocketIndex;				// socket����
	short	mSocketError;				// socket����
};


#define MAX_PLAYER_NAME_LEN		32		// ���������󳤶�

enum EmPlayerStatus
{
	emPlayerStatus_None			= 0,	// ��״̬
	emPlayerStatus_Loading		= 1,	// ����״̬
	emPlayerStatus_Gameing		= 2,	// ��Ϸ״̬
	emPlayerStatus_Exiting		= 3,	// �˳�״̬
};
#endif