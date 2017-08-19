syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum ITEM_MODULE_MSG_ID
{
	ID_ITEM_MODULE_ERROR					= 0x0;
	ID_S2C_NOTIYF_PLAYER_MONEY_UPDATE		= 0x0800;
}


message CMessagePlayerMoneyUpdateNotify
{
	uint32	Money				= 1;		// 玩家当前的金钱
}

