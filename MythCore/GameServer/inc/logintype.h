#ifndef __LOGINTYPE_H__
#define __LOGINTYPE_H__
#include "servercommon.h"
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
#define PLAYER_NAME_RESERVE		6		// ������ֺϷ�Ԥ��

#define MIN_PLAYER_NAME_CHAR	2		// ���������С�ַ�
#define MAX_PLAYER_NAME_CHAR	10		// �����������ַ�

enum EmPlayerStatus
{
	emPlayerStatus_None			= 0,	// ��״̬
	emPlayerStatus_Loading		= 1,	// ����״̬
	emPlayerStatus_Gameing		= 2,	// ��Ϸ״̬
	emPlayerStatus_Exiting		= 3,	// �˳�״̬
};

enum EmPlayerSaveStatus
{
	emSaveStatus_None			= 0x0000,	// ���û�б���ɹ��κ�һ��������
	emSaveStatus_Info			= 0x0001,	// �Ѿ�����ɹ���Ҫ��Ϣ����
	emSaveStatus_BaseProperty	= 0x0002,	// �Ѿ�����ɹ��������Բ���
	emSaveStatusAll				= 0x0003,	// ���еĶ��Ѿ��������
};

enum EmPlayerLoadStatus
{
	emLoadStatus_None			= 0x0000,	// ���û�м��سɹ��κ�һ��������
	emLoadStatus_Info			= 0x0001,	// �Ѿ����سɹ���Ҫ��Ϣ����
	emLoadStatus_BaseProperty	= 0x0002,	// �Ѿ����سɹ��������Բ���
	emLoadStatusAll				= 0x0003,	// ���еĶ��Ѿ��������
};

#define	MAX_SERVER_ID			10000			// ���ķ�����ID
#define MAX_ROLE_PER_SERVER		200000			// ÿ�����������20���ɫ
#define BAT_LOAD_ROLE_ID_NUM	2000			// �������ط����ɫID����

enum EmPlayerSex
{
	EmPlayerSex_Male			= 0,		// ����
	EmPlayerSex_Female			= 1,		// Ů��
	EmPlayerSex_None			= 2,		// ���Ա�
};
#endif