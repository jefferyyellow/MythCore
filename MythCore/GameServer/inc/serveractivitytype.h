#ifndef __SERVERACTIVITYTYPE_H__
#define __SERVERACTIVITYTYPE_H__


#define MAKE_ACT_UNIQUE_ID(nType, nSubType, ID) (nType << 24 | nSubType << 16 | ID)
#define MAX_SERVER_ACT_NUM	128
enum EmSvrActState
{
	emSvrActState_End		= 0,		// ����״̬
	emSvrActState_Start		= 1,		// ��ʼ״̬
	emSvrActState_Prize		= 2,		// ����״̬
};

// ��Ӫ�
enum EmSvrActType
{
	emSvrActType_None			= 0,		// ������
	emSvrActType_CumRecharge	= 1,		// �ۼƳ�ֵ
	emSvrActType_CumConsume		= 2,		// �ۼ�����
	emSvrActType_Rank			= 3,		// ���л
	emServerActTypeMax
};

// ���а�������
enum EmRankActSubType
{
	emRankActSubType_None		= 0,		// ������
	emRankActSubType_Level		= 1,		// �ȼ����а�
	emRankActSubType_Power		= 2,		// ս�����а�
};

/// ���������
enum EmFuncType
{
	emFuncType_None				= 0,		// ������
	emFuncType_End				= 1,		// ���������
	emFuncType_ClearPlayerData	= 2,		// ����������ݺ���
	emFuncType_RefreshProcess	= 3,		// ˢ�»����
};
#endif