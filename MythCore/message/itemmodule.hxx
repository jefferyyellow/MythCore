syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum ITEM_MODULE_MSG_ID
{
	ID_ITEM_MODULE_ERROR					= 0x0;
	ID_S2C_NOTIYF_PLAYER_MONEY_UPDATE		= 0x0800;	// ������->�ͻ��� ��ҽ�Ǯ����֪ͨ
	ID_S2C_NOTIYF_PLAYER_DIAMOND_UPDATE		= 0x0801;	// ������->�ͻ��� ���
};

// ��ҽ�Ǯ����֪ͨ
message CMessagePlayerMoneyUpdateNotify
{
	uint32	Money				= 1;		// ��ҵ�ǰ�Ľ�Ǯ
}

// �����ʯ����֪ͨ
message CMessagePlayerDiamondUpdateNotify
{
	uint32	Diamond				= 1;		// ��ҵ�ǰ����ʯ
}