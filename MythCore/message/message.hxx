syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum MESSAGE_MODULE_ID
{
	MESSAGE_MODULE_LOGIN			= 0x0000;		// ������(0000 00	00 0000 0000)	��¼ģ��
	MESSAGE_MODULE_PROPERTY			= 0x0400;		// ������(0000 01	00 0000 0000)	��������ģ��
	MESSAGE_MODULE_ITEM				= 0x0800;		// ������(0000 10	00 0000 0000)	����ģ��
	MESSAGE_MODULE_MAP				= 0x0C00;		// ������(0000 11	00 0000 0000)	��ͼģ��
	MESSAGE_MODULE_TASK				= 0x1000;		// ������(0001 00	00 0000 0000)	����ģ��
	MESSAGE_MODULE_SKILL			= 0x1400;		// ������(0001 01	00 0000 0000)	����ģ��
	MESSAGE_MODULE_CHAT				= 0x1800;		// ������(0001 10	00 0000 0000)	����ģ��
	MESSAGE_MODULE_SERVER_ACT		= 0x1C00;		// ������(0001 11	00 0000 0000)	�����ģ��
};

