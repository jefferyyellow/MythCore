syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum LOGIN_MODULE_MSG_ID
{
	ID_LOGIN_MODULE_ERROR			= 0x0;
	ID_C2S_REQUEST_LOGIN			= 0x01;				// ��ҵ�½����
	ID_S2C_RESPONSE_LOGIN			= 0x02;				// ��ҵ�½��Ӧ
	ID_C2S_REQUEST_CREATE_ROLE		= 0x03;				// ��Ҵ�����ɫ����
	ID_S2C_RESPONSE_CREATE_ROLE		= 0x04;				// ��Ҵ�����ɫ��Ӧ
	ID_C2S_REQUEST_ENTER_SCENE		= 0x05;				// ��ҽ�����Ϸ����
	ID_S2C_RESPONSE_ENTER_SCENE		= 0x06;				// ��ҽ�����Ϸ��Ӧ
}


message CMessageLoginRequest
{
	string  Name					= 1;
	uint32	ChannelID				= 2;
	uint32	ServerID				= 3;
}

message CMessageLoginResponse
{
	uint32	AccountID				= 1;
	uint32	RoleID					= 2;
	uint32	ChannelID				= 3;
	uint32	ServerID					= 4;		
}

message CMessageCreateRoleRequest
{
	uint32	AccountID				= 1;
	uint32	ChannelID				= 2;
	uint32	ServerID					= 3;
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
	uint32	ServerID				= 4;
}

message CMessageEnterSceneResponse
{
	uint32 Result					= 2;
}