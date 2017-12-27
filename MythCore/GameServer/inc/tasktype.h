#ifndef __TASKTYPE_H__
#define __TASKTYPE_H__

#define		MAX_PLAYER_TASK_NUM			20			// ���Ŀɽ�������Ŀ
#define		MAX_PLAYER_TASK_PARAM		2			// �������������Ŀ

#define MAX_ACCEPT_CONDITION_PARAM		4			// ���Ľ�����������
#define MAX_COMPLETE_CONDITION_PARAM	4			// ���������������
#define MAX_TASK_REWARD_PARAM			4			// ��������������
#define MAX_TASK_INVALID_PARAM			2			// ��������ʧЧ����

#define	MAX_ACCEPT_CONDITION			4			// ������������
#define MAX_COMPLETE_CONDITION			4			// �����������
#define MAX_CLOSE_CONDITION				4			// ����ʧЧ����
#define MAX_TASK_PRIZE_NUM				8			// ���Ľ�����Ŀ
#define MAX_TASK_ID						8192		// �����������ID



enum EmAcceptCondition
{
	emAccept_None						= 0,		// ��
	emAccept_PreTask					= 1,		// ǰ������				����0��ǰ������ID
	emAccept_Level						= 2,		// �ȼ�					����0����С�ȼ�
};

enum EmCompleteCondition
{
	emComplete_None						= 0,		// ��
	emComplete_KillOgre					= 1,		// ɱ�ּ���(֪ͨǰ��)	����0������ID��		����1��������Ŀ
	emComplete_HoldItem					= 2,		// ӵ�е���				����0������ID��		����1��������Ŀ
	emComplete_Level					= 3,		// �ȼ�					����0����ҵȼ�		
	emComplete_PassFB					= 4,		// ͨ�ظ���(֪ͨǰ��)	����0����ͼID		����1����������			����2����������		����3��ͨ������
	emComplete_Collect					= 5,		// �ɼ�(֪ͨǰ��)		����0������ID		����1��������Ŀ
	emComplete_Talk						= 6,		// �Ի�
};

enum EmCloseCondition
{
	emClose_None						= 0,		// ��
	emClose_Prestige					= 1,		// ����
	emClose_Level						= 2,		// �ȼ�
	emClose_PreTask						= 3,		// ǰ������
};

enum EmTaskPrizeType
{
	emTaskPrize_None					= 0,		// ��
	emTaskPrize_Item					= 1,		// ����(���ң�
};

#endif