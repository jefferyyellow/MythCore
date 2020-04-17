syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息
enum LOGIN_MODULE_MSG_ID
{
	ID_LOGIN_MODULE_ERROR			= 0x0;
	ID_C2S_REQUEST_LOGIN			= 0x01;				// 登陆请求
	ID_S2C_RESPONSE_LOGIN			= 0x02;				// 登陆回应
	ID_C2S_REQUEST_CREATE_ROLE		= 0x03;				// 创建角色请求
	ID_S2C_RESPONSE_CREATE_ROLE		= 0x04;				// 创建角色回应
	ID_C2S_REQUEST_ENTER_SCENE		= 0x05;				// 进入游戏请求
	ID_S2C_RESPONSE_ENTER_SCENE		= 0x06;				// 进入游戏回应
};

/// 登录请求 ID_C2S_REQUEST_LOGIN
message CLoginRequest
{
	string  Name					= 1;				// 账号名称
	uint32	ChannelID				= 2;				// 渠道ID
	uint32	ServerID				= 3;				// 服务器ID
}

/// 登录回应 ID_S2C_RESPONSE_LOGIN
message CLoginResponse
{
	uint32	AccountID				= 1;				// 账号ID
	uint32	RoleID					= 2;				// 角色ID
	uint32	ChannelID				= 3;				// 渠道ID
	uint32	ServerID				= 4;				// 服务器ID
}

/// 创建角色请求 ID_C2S_REQUEST_CREATE_ROLE
message CCreateRoleRequest
{
	uint32	AccountID				= 1;
	uint32	ChannelID				= 2;
	uint32	ServerID				= 3;
	string	RoleName				= 4;
	uint32	Sex						= 5;
	uint32	Metier					= 6;
}

/// 创建角色回应 ID_S2C_RESPONSE_CREATE_ROLE
message CCreateRoleResponse
{
	uint32	Result					= 1;
	uint32	RoleID					= 2;
}

/// 进入游戏请求 ID_C2S_REQUEST_ENTER_SCENE
message CEnterSceneRequest
{
	uint32	RoleID					= 1;
	uint32	AccountID				= 2;
	uint32	ChannelID				= 3;
	uint32	ServerID				= 4;
}

/// 进入游戏回应 ID_S2C_RESPONSE_ENTER_SCENE
message CEnterSceneResponse
{
	uint32 Result					= 1;
}