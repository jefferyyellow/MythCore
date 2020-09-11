#ifndef __INSTANCETYPE_H__
#define __INSTANCETYPE_H__

#define MAX_INSTANCE_MAP_NUM	8				// ��������ͼ��
// ��ͨ����500 �������� 500
// �������͵ĸ��� 200���ܹ����Էֳ� 7����
#define MAX_INSTANCE_ID			2000			// ���ĸ���ID

enum EmInstanceStatus
{
	emInstanceStatus_None	= 0,			// ��
	emInstanceStatus_Init	= 1,			// ��ʼ��
	emInstanceStatus_Play	= 2,			// ��ʼ��
	emInstanceStatus_End	= 3,			// ����
	emInstanceStatus_Destory= 4,			// ����
};

enum EmInstanceType
{
	emInstance_None			= 0,			// ��
	// ���˸�������
	emInstance_Common		= 1,			// ��ͨ���˸���
	// ���˸�������
	emInstance_Team			= 2,			// ��ͨ��Ӹ���
};

enum EmInstanceEvent
{
	emInstEvent_None			= 0,			// ��
	emInstEvent_OgreDead		= 1,			// ��������
	emInstEvent_PlayerDead		= 2,			// �������
	emInstEvent_PlayerHP		= 3,			// ���Ѫ���仯
	emInstEventMax
};
#endif