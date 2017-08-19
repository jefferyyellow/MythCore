syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum PROPERTY_MODULE_MSG_ID
{
	ID_PROPERTY_MODULE_ERROR			= 0x0;
	ID_S2C_NOTIYF_PLAYER_LEVEL_UP		= 0x0400;
}


message CMessagePlayerLevelUpNotify
{
	uint32	Level				= 1;		// 玩家等级
}

