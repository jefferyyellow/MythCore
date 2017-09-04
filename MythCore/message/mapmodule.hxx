syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum MAP_MODULE_MSG_ID
{
	ID_MAP_MODULE_ERROR						= 0x0;
	ID_S2C_NOTIYF_ENTITY_MOVE				= 0x0C00;	// 服务器->客户端 实体移动通知
	ID_S2C_NOTIYF_CREATE_PLAYER_LIST		= 0x0C01;	// 服务器->客户端 创建玩家列表通知
	ID_S2C_NOTIYF_CREATE_NPC_LIST			= 0x0C02;	// 服务器->客户端 创建NPC列表通知
};

message CMessageEntityMoveNotify
{
	uint32	EntityID						= 1;		// 实体的实体ID
	uint32	DesPosX							= 2;		// 实体的移动坐标X
	uint32	DesPosY							= 3;		// 实体的移动坐标Y
}

message CMessagePlayerSceneInfo
{
	uint32	EntityID						= 1;		// 实体的实体ID
	uint32	PosX							= 2;		// 位置X坐标
	uint32	PosY							= 3;		// 位置Y坐标
}

message PBPlayerSceneInfo
{
	uint32	EntityID						= 1;		// 实体的实体ID
	uint32	PosX							= 2;		// 位置X坐标
	uint32	PosY							= 3;		// 位置Y坐标
}

message CMessageCreatePlayerListNotify
{
	repeated PBPlayerSceneInfo	PlayerInfo	= 1;		// 玩家信息
}

message PBNpcSceneInfo
{
	uint32	EntityID						= 1;		// 实体的实体ID
	uint32	TempID							= 2;		// 模板ID
	uint32	PosX							= 3;		// 位置X坐标
	uint32	PosY							= 4;		// 位置Y坐标
}

message CMessageCreateNPCListNotify
{
	repeated PBNpcSceneInfo		NPCInfo		= 1;		// NPC列表信息	
}