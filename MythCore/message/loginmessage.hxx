syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum LOGIN_MSG_ID
{
	ID_ERROR						= 0x0;
	ID_C2S_REQUEST_LOGIN			= 0x01;
	ID_S2C_RESPONSE_LOGIN			= 0x02;
	ID_C2S_REQUEST_CREATE_ROLE		= 0x03;
	ID_S2C_RESPONSE_CREATE_ROLE		= 0x04;
	ID_C2S_REQUEST_ENTER_SCENE		= 0x05;
	ID_S2C_RESPONSE_ENTER_SCENE		= 0x06;
}


message CMessageLoginRequest
{
	string  Name					= 1;
	uint32	ChannelID				= 2;
	uint32	WorldID					= 3;
}

message CMessageLoginResponse
{
	uint32	AccountID				= 1;
	uint32	RoleID					= 2;
	uint32	ChannelID				= 3;
	uint32	WorldID					= 4;		
}

message CMessageCreateRoleRequest
{
	uint32	AccountID				= 1;
	uint32	ChannelID				= 2;
	uint32	WorldID					= 3;
	string	RoleName				= 4;
}

message CMessageCreateRoleResponse
{
	uint32	Result					= 1;
	uint32	RoleID					= 2;
}

message CMessageEnterSceneRequest
{
	uint32	RoleID					= 1;
	uint32	AccountID				= 2;
	uint32	ChannelID				= 3;
	uint32	WorldID					= 4;
}

message CMessageEnterSceneResponse
{
	uint32 Result					= 2;
}