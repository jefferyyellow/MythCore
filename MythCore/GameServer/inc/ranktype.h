#ifndef __RANKTYPE_H__
#define __RANKTYPE_H__
enum EmRankType
{
	emRankType_None				= -1,	// ������
	emRankType_Level			= 0,	// �ȼ����а�
	emRankTypeMax
};

#define MAX_RANK_NUM			1000
#define MAX_RANK_SHOW_NUM		100		// ���а������ʾ����
#define MAX_RANK_SHOW_CACHE_NUM	200		// ���а������ʾ��������(Ϊ�˷�ֹ����MAX_RANK_SHOW_NUMû�˲���)

#endif