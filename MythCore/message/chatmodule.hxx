syntax = "proto3";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum CHAT_MODULE_MSG_ID
{
	ID_CHAT_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_CHAT						= 0x1800;	// �����������
	ID_S2C_RESPONSE_CHAT					= 0x1801;	// ��������Ӧ
	ID_S2C_NOTIFY_CHAT						= 0x1802;	// �������֪ͨ
	ID_S2C_NOTIFY_WORLD_NEWS				= 0x1803;	// ��������֪ͨ
};

// ����������� ID_C2S_REQUEST_CHAT
message CChatRequest
{
	uint32	Channel		= 1;	// Ƶ��
	string	Content		= 2;	// ����
}

// ��������Ӧ ID_S2C_RESPONSE_CHAT
message CChatResponse
{
	uint32	Result		= 1;	// ���
}

// �������֪ͨ ID_S2C_NOTIFY_CHAT
message CChatNotify
{
	uint32	PlayerID	= 1;	// ���ID
	string	PlayerName	= 2;	// �������
	uint32	Channel		= 3;	// Ƶ��
	string	Content		= 4;	// ����
}

// �������ŵ������Ϣ
message PBNewsRole
{
	uint32	PlayerID	= 1;	// ���ID
	string	PlayerName	= 2;	// �������
}

// �������Ų���
message PBNewsParam
{
	repeated uint32		Param		= 1;
	repeated string		StrParam	= 2;
	repeated PBNewsRole	Role		= 3;
}

// ��������֪ͨ ID_S2C_NOTIFY_WORLD_NEWS
message CWorldNewsNotify
{
	uint32				Type		= 1;
	PBNewsRole			Role		= 2;
	PBNewsParam			NewsParam	= 3;
}