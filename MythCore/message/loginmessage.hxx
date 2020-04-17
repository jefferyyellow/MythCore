syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ
enum LOGIN_MODULE_MSG_ID
{
	ID_LOGIN_MODULE_ERROR			= 0x0;
	ID_C2S_REQUEST_LOGIN			= 0x01;				// ��½����
	ID_S2C_RESPONSE_LOGIN			= 0x02;				// ��½��Ӧ
	ID_C2S_REQUEST_CREATE_ROLE		= 0x03;				// ������ɫ����
	ID_S2C_RESPONSE_CREATE_ROLE		= 0x04;				// ������ɫ��Ӧ
	ID_C2S_REQUEST_ENTER_SCENE		= 0x05;				// ������Ϸ����
	ID_S2C_RESPONSE_ENTER_SCENE		= 0x06;				// ������Ϸ��Ӧ
};

/// ��¼���� ID_C2S_REQUEST_LOGIN
message CLoginRequest
{
	string  Name					= 1;				// �˺�����
	uint32	ChannelID				= 2;				// ����ID
	uint32	ServerID				= 3;				// ������ID
}

/// ��¼��Ӧ ID_S2C_RESPONSE_LOGIN
message CLoginResponse
{
	uint32	AccountID				= 1;				// �˺�ID
	uint32	RoleID					= 2;				// ��ɫID
	uint32	ChannelID				= 3;				// ����ID
	uint32	ServerID				= 4;				// ������ID
}

/// ������ɫ���� ID_C2S_REQUEST_CREATE_ROLE
message CCreateRoleRequest
{
	uint32	AccountID				= 1;
	uint32	ChannelID				= 2;
	uint32	ServerID				= 3;
	string	RoleName				= 4;
	uint32	Sex						= 5;
	uint32	Metier					= 6;
}

/// ������ɫ��Ӧ ID_S2C_RESPONSE_CREATE_ROLE
message CCreateRoleResponse
{
	uint32	Result					= 1;
	uint32	RoleID					= 2;
}

/// ������Ϸ���� ID_C2S_REQUEST_ENTER_SCENE
message CEnterSceneRequest
{
	uint32	RoleID					= 1;
	uint32	AccountID				= 2;
	uint32	ChannelID				= 3;
	uint32	ServerID				= 4;
}

/// ������Ϸ��Ӧ ID_S2C_RESPONSE_ENTER_SCENE
message CEnterSceneResponse
{
	uint32 Result					= 1;
}