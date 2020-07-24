syntax = "proto3";
import "common.hxx";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum PROPERTY_MODULE_MSG_ID
{
	ID_PROPERTY_MODULE_ERROR				= 0x0;
	ID_S2C_NOTIYF_LEVEL_UP					= 0x0400;		// 玩家升级通知
	ID_C2S_REQUEST_GM_COMMAND				= 0x0401;		// 玩家GM命令请求
	ID_S2C_RESPONSE_GM_COMMAND				= 0x0402;		// 玩家GM命令回应
	ID_S2C_NOTIFY_OBTAIN_EXP				= 0x0403;		// 获得经验通知
	ID_S2C_NOTIFY_OBTAIN_VIP_EXP			= 0x0404;		// 获得VIP经验通知
	ID_C2S_REQUEST_LEAVE_GAME				= 0x0405;		// 离开游戏请求
	ID_S2C_RESPONSE_LEAVE_GAME				= 0x0406;		// 离开游戏回应
	ID_S2C_NOTIFY_PLAYER_BASE_INFO			= 0x0407;		// 玩家基本信息通知
	ID_S2C_NOTIFY_PLAYER_ITEM_INFO			= 0x0408;		// 玩家道具信息通知
	ID_S2C_NOTIFY_PLAYER_TASK_INFO			= 0x0409;		// 玩家任务信息通知
	ID_S2C_NOTIFY_PLAYER_SKILL_INFO			= 0x040A;		// 玩家技能信息通知
	ID_C2S_REQUEST_GET_PLAYER_PROPERTY		= 0x040B;		// 得到玩家属性请求
	ID_S2C_RESPONSE_GET_PLAYER_PROPERTY		= 0x040C;		// 得到玩家属性的回应
	ID_C2S_REQUEST_HEART_BEAT				= 0x040D;		// 玩家心跳的请求
	ID_S2C_RESPONSE_HEART_BEAT				= 0x040E;		// 玩家心跳的回应
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
	uint32 Result				= 1;		// 结果
}

/// 玩家基本信息通知 ID_S2C_NOTIFY_PLAYER_BASE_INFO
message CPlayerBaseInfoNotify
{
	uint32 RoleID				= 1;			// 角色ID
	uint32 EntityID				= 2;			// 实体ID
	uint32 LineID				= 3;			// 地图线ID
	uint32 MapID				= 4;			// 地图ID
	uint32 MapIndex				= 5;			// 地图索引
	uint32 PosX					= 6;			// 位置X
	uint32 PosY					= 7;			// 位置Y
	uint32 Level				= 8;			// 等级
	uint64 Exp					= 9;			// 经验
	uint32 VipLevel				= 10;			// Vip等级
	uint32 VipExp				= 11;			// Vip经验
}

/// 玩家道具信息通知 ID_S2C_NOTIFY_PLAYER_ITEM_INFO
message CPlayerItemInfoNotify
{
	PBItemList Bag				= 1;			// 包裹
}

/// 玩家任务信息通知 ID_S2C_NOTIFY_PLAYER_TASK_INFO
message CPlayerTaskInfoNotify
{
	PBTaskList TaskList			= 1;			// 任务列表
}

/// 玩家技能信息通知 ID_S2C_NOTIFY_PLAYER_SKILL_INFO
message CPlayerSkillInfoNotify
{
	
}

/// 得到玩家属性请求 ID_C2S_REQUEST_GET_PLAYER_PROPERTY
message CGetPlayerPropertyRequest
{

}

/// 得到玩家属性回应 ID_S2C_RESPONSE_GET_PLAYER_PROPERTY
message CGetPlayerPropertyResponse
{
	repeated uint32		PropertyValue	= 1;	// 属性值
}

/// 玩家心跳请求 ID_C2S_REQUEST_HEART_BEAT
message CHeartBeatRequest
{
}

/// 玩家心跳回应 ID_S2C_RESPONSE_HEART_BEAT
message CHeartBeatResponse
{
	uint32	ServerTime					= 1;	// 服务器时间
}