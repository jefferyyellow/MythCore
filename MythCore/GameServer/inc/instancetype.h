#ifndef __INSTANCETYPE_H__
#define __INSTANCETYPE_H__

#define MAX_INSTANCE_MAP_NUM 4				// ��������ͼ��

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
	emInstance_Team			= 32,			// ��ͨ��Ӹ���
};


#endif