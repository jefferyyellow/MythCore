syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum PROPERTY_MODULE_MSG_ID
{
	ID_PROPERTY_MODULE_ERROR			= 0x0;
	ID_S2C_NOTIYF_PLAYER_LEVEL_UP		= 0x0400;
}


message CMessagePlayerLevelUpNotify
{
	uint32	Level				= 1;		// ��ҵȼ�
}

