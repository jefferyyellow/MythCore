syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum PROPERTY_MODULE_MSG_ID
{
	ID_PROPERTY_MODULE_ERROR			= 0x0;
	ID_S2C_NOTIYF_LEVEL_UP				= 0x0400;		// 玩家升级通知
	ID_C2S_REQUEST_GM_COMMAND			= 0x0401;		// 玩家GM命令请求
	ID_S2C_RESPONSE_GM_COMMAND			= 0x0402;		// 玩家GM命令回应
	ID_S2C_NOTIFY_OBTAIN_EXP			= 0x0403;		// 获得经验通知
	ID_S2C_NOTIFY_OBTAIN_VIP_EXP		= 0x0404;		// 获得VIP经验通知
	ID_C2S_REQUEST_LEAVE_GAME			= 0x0405;		// 离开游戏请求
	ID_S2C_RESPONSE_LEAVE_GAME			= 0x0406;		// 离开游戏回应
}

/// 玩家升级通知 ID_S2C_NOTIYF_LEVEL_UP
message CLevelUpNotify
{
	uint32	Level				= 1;		// 玩家等级
}

/// GM命令请求 ID_C2S_REQUEST_GM_COMMAND
message CGMCommandRequest
{
	string CommandString		= 1;		// 命令字符串 
}

/// GM命令回应 ID_S2C_RESPONSE_GM_COMMAND
message CGMCommandResponse
{
	uint32 Result				= 1;		// 结果
}

/// 获得经验通知 ID_S2C_NOTIFY_OBTAIN_EXP
message CObtainExpNotify
{
	uint32 Exp					= 1;		// 获得的经验
}

/// 获得VIP经验通知 ID_S2C_NOTIFY_OBTAIN_VIP_EXP
message CObtainVIPExpNotify
{

}

/// 玩家离开游戏的请求 ID_C2S_REQUEST_LEAVE_GAME
message CLeaveGameRequest
{
	
}

/// 玩家离开游戏的回应 ID_S2C_RESPONSE_LEAVE_GAME
message CLeaveGameResponse 
{
	
}