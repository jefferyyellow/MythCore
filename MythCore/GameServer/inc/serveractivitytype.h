#ifndef __SERVERACTIVITYTYPE_H__
#define __SERVERACTIVITYTYPE_H__

#define ACTIVITY_PRIZE_NUM		4	// �������Ŀ
#define PHASE_ACTIVITY_NUM		30	// �׶����������

enum EmServerActState
{
	emServerActState_End		= 0,		// ����״̬
	emServerActState_Start		= 1,		// ��ʼ״̬
	emServerActState_Prize		= 2,		// ����״̬
};


enum EmServerActType
{
	emServerActType_None		= 0,		// ������
	emServerActType_Phase		= 1,		// �����׶λ��
};

#endif