#ifndef __INSTANCETYPE_H__
#define __INSTANCETYPE_H__

#define MAX_INSTANCE_MAP_NUM 10				// ��������ͼ��

enum EmInstanceStatus
{
	emInstanceStatus_None	= 0,			// ��
	emInstanceStatus_Init	= 1,			// ��ʼ��
	emInstanceStatus_Play	= 2,			// ��ʼ��
	emInstanceStatus_End	= 3,			// ����
	emInstanceStatus_Destory= 4,			// ����
};
#endif