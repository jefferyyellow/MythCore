syntax = "proto3";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum DAILY_ACT_MODULE_MSG_ID
{
	ID_DAILY_ACT_MODULE_ERROR				= 0x0;
	ID_S2C_NOTIFY_ACTIVITY_START			= 0x2000;
};

// �����֪ͨ ID_S2C_NOTIFY_ACTIVITY_START
message CActivityStatusNotify
{
	uint32	ActivityID			= 1;
	uint32	Status				= 2;
}
