syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum MESSAGE_MODULE_ID
{
	MESSAGE_MODULE_LOGIN			= 0x0000;		// ������(0000 00	00 0000 0000)
	MESSAGE_MODULE_PROPERTY			= 0x0400;		// ������(0000 01	00 0000 0000)
	MESSAGE_MODULE_ITEM				= 0x0800;		// ������(0000 10	00 0000 0000)
	MESSAGE_MODULE_MAP				= 0x0C00;		// ������(0000 11	00 0000 0000)

};
