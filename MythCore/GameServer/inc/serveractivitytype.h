#ifndef __SERVERACTIVITYTYPE_H__
#define __SERVERACTIVITYTYPE_H__

#define ACTIVITY_PRIZE_NUM		6	// �������Ŀ
#define PHASE_ACTIVITY_NUM		30	// �׶����������

#define MAKE_ACT_UNIQUE_ID(nType, nSubType, ID) (nType << 24 | nSubType << 16 | ID)

enum EmServerActState
{
	emServerActState_End		= 0,		// ����״̬
	emServerActState_Start		= 1,		// ��ʼ״̬
	emServerActState_Prize		= 2,		// ����״̬
};


enum EmServerActType
{
	emServerActType_None		= 0,		// ������
	emServerActType_Phase		= 1,		// �����׶λ
	emServerActTypeMax
};

#endif