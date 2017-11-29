syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum MAP_MODULE_MSG_ID
{
	ID_MAP_MODULE_ERROR						= 0x0;
	ID_S2C_NOTIYF_ENTITY_MOVE				= 0x0C00;	// 实体移动通知
	ID_S2C_NOTIYF_CREATE_PLAYER_LIST		= 0x0C01;	// 创建玩家列表通知
	ID_S2C_NOTIYF_CREATE_NPC_LIST			= 0x0C02;	// 创建NPC列表通知
	ID_S2C_NOTIYF_DESTROY_ENTITY			= 0x0C03;	// 销毁实体通知
	ID_C2S_REQUEST_PLAYER_MOVE				= 0x0C04;	// 玩家移动的请求
	ID_S2C_RESPONSE_PLAYER_MOVE				= 0x0C05;	// 玩家移动回应
	ID_C2S_REQUEST_PLAYER_TELEPORT			= 0x0C06;	// 玩家传送请求
	ID_S2C_RESPONSE_PLAYER_TELEPORT			= 0x0C07;	// 玩家传送回应
};

// 实体移动通知 ID_S2C_NOTIYF_ENTITY_MOVE
message CEntityMoveNotify
{
	uint32	EntityID						= 1;		// 实体的实体ID
	uint32	DesPosX							= 2;		// 实体的移动坐标X
	uint32	DesPosY							= 3;		// 实体的移动坐标Y
}

// 玩家场景信息
message PBPlayerSceneInfo
{
	uint32	EntityID						= 1;		// 实体的实体ID
	uint32	PosX							= 2;		// 位置X坐标
	uint32	PosY							= 3;		// 位置Y坐标
}

// 创建玩家列表通知 ID_S2C_NOTIYF_CREATE_PLAYER_LIST
message CCreatePlayerListNotify
{
	repeated PBPlayerSceneInfo	PlayerInfo	= 1;		// 玩家信息
}

// NPC场景信息
message PBNpcSceneInfo
{
	uint32	EntityID						= 1;		// 实体的实体ID
	uint32	TempID							= 2;		// 模板ID
	uint32	PosX							= 3;		// 位置X坐标
	uint32	PosY							= 4;		// 位置Y坐标
}

// 创建NPC列表通知 ID_S2C_NOTIYF_CREATE_NPC_LIST
message CCreateNPCListNotify
{
	repeated PBNpcSceneInfo		NPCInfo		= 1;		// NPC列表信息	
}

// 移除实体通知 ID_S2C_NOTIYF_DESTROY_ENTITY
message CDestroyEntityNotify
{
	repeated uint32				EntityID	= 1;		// 实体列表
}

// 玩家移动请求	ID_C2S_REQUEST_PLAYER_MOVE
message CPlayerMoveRequest
{
	uint32	DesPosX							= 1;		// 目标位置X坐标
	uint32	DesPosY							= 2;		// 目标位置Y坐标
}

// 玩家移动回应 ID_S2C_RESPONSE_PLAYER_MOVE
message CPlayerMoveResponse
{
	uint32	Result							= 1;		// 结果
}

// 玩家传送请求 ID_C2S_REQUEST_PLAYER_TELEPORT
message CPlayerTeleportRequest
{
	uint32	MapID							= 1;		// 传送去的地图
}

// 玩家传送回应 ID_S2C_RESPONSE_PLAYER_TELEPORT
message CPlayerTeleportResponse
{
	uint32	Result							= 1;		// 结果
}