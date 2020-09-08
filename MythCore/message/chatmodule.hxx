syntax = "proto3";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum CHAT_MODULE_MSG_ID
{
	ID_CHAT_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_CHAT						= 0x1800;	// 玩家聊天请求
	ID_S2C_RESPONSE_CHAT					= 0x1801;	// 玩家聊天回应
	ID_S2C_NOTIFY_CHAT						= 0x1802;	// 玩家聊天通知
	ID_S2C_NOTIFY_WORLD_NEWS				= 0x1803;	// 世界新闻通知
};

// 玩家聊天请求 ID_C2S_REQUEST_CHAT
message CChatRequest
{
	uint32	Channel		= 1;	// 频道
	string	Content		= 2;	// 内容
}

// 玩家聊天回应 ID_S2C_RESPONSE_CHAT
message CChatResponse
{
	uint32	Result		= 1;	// 结果
}

// 玩家聊天通知 ID_S2C_NOTIFY_CHAT
message CChatNotify
{
	uint32	PlayerID	= 1;	// 玩家ID
	string	PlayerName	= 2;	// 玩家名字
	uint32	Channel		= 3;	// 频道
	string	Content		= 4;	// 内容
}

// 世界新闻的玩家信息
message PBNewsRole
{
	uint32	PlayerID	= 1;	// 玩家ID
	string	PlayerName	= 2;	// 玩家姓名
}

// 世界新闻参数
message PBNewsParam
{
	repeated uint32		Param		= 1;
	repeated string		StrParam	= 2;
	repeated PBNewsRole	Role		= 3;
}

// 世界新闻通知 ID_S2C_NOTIFY_WORLD_NEWS
message CWorldNewsNotify
{
	uint32				Type		= 1;
	PBNewsRole			Role		= 2;
	PBNewsParam			NewsParam	= 3;
}