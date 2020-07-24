syntax = "proto3";
import "common.hxx";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum PROPERTY_MODULE_MSG_ID
{
	ID_PROPERTY_MODULE_ERROR				= 0x0;
	ID_S2C_NOTIYF_LEVEL_UP					= 0x0400;		// �������֪ͨ
	ID_C2S_REQUEST_GM_COMMAND				= 0x0401;		// ���GM��������
	ID_S2C_RESPONSE_GM_COMMAND				= 0x0402;		// ���GM�����Ӧ
	ID_S2C_NOTIFY_OBTAIN_EXP				= 0x0403;		// ��þ���֪ͨ
	ID_S2C_NOTIFY_OBTAIN_VIP_EXP			= 0x0404;		// ���VIP����֪ͨ
	ID_C2S_REQUEST_LEAVE_GAME				= 0x0405;		// �뿪��Ϸ����
	ID_S2C_RESPONSE_LEAVE_GAME				= 0x0406;		// �뿪��Ϸ��Ӧ
	ID_S2C_NOTIFY_PLAYER_BASE_INFO			= 0x0407;		// ��һ�����Ϣ֪ͨ
	ID_S2C_NOTIFY_PLAYER_ITEM_INFO			= 0x0408;		// ��ҵ�����Ϣ֪ͨ
	ID_S2C_NOTIFY_PLAYER_TASK_INFO			= 0x0409;		// ���������Ϣ֪ͨ
	ID_S2C_NOTIFY_PLAYER_SKILL_INFO			= 0x040A;		// ��Ҽ�����Ϣ֪ͨ
	ID_C2S_REQUEST_GET_PLAYER_PROPERTY		= 0x040B;		// �õ������������
	ID_S2C_RESPONSE_GET_PLAYER_PROPERTY		= 0x040C;		// �õ�������ԵĻ�Ӧ
	ID_C2S_REQUEST_HEART_BEAT				= 0x040D;		// �������������
	ID_S2C_RESPONSE_HEART_BEAT				= 0x040E;		// ��������Ļ�Ӧ
}

/// �������֪ͨ ID_S2C_NOTIYF_LEVEL_UP
message CLevelUpNotify
{
	uint32	Level				= 1;		// ��ҵȼ�
}

/// GM�������� ID_C2S_REQUEST_GM_COMMAND
message CGMCommandRequest
{
	string CommandString		= 1;		// �����ַ��� 
}

/// GM�����Ӧ ID_S2C_RESPONSE_GM_COMMAND
message CGMCommandResponse
{
	uint32 Result				= 1;		// ���
}

/// ��þ���֪ͨ ID_S2C_NOTIFY_OBTAIN_EXP
message CObtainExpNotify
{
	uint32 Exp					= 1;		// ��õľ���
}

/// ���VIP����֪ͨ ID_S2C_NOTIFY_OBTAIN_VIP_EXP
message CObtainVIPExpNotify
{

}

/// ����뿪��Ϸ������ ID_C2S_REQUEST_LEAVE_GAME
message CLeaveGameRequest
{
	
}

/// ����뿪��Ϸ�Ļ�Ӧ ID_S2C_RESPONSE_LEAVE_GAME
message CLeaveGameResponse 
{
	uint32 Result				= 1;		// ���
}

/// ��һ�����Ϣ֪ͨ ID_S2C_NOTIFY_PLAYER_BASE_INFO
message CPlayerBaseInfoNotify
{
	uint32 RoleID				= 1;			// ��ɫID
	uint32 EntityID				= 2;			// ʵ��ID
	uint32 LineID				= 3;			// ��ͼ��ID
	uint32 MapID				= 4;			// ��ͼID
	uint32 MapIndex				= 5;			// ��ͼ����
	uint32 PosX					= 6;			// λ��X
	uint32 PosY					= 7;			// λ��Y
	uint32 Level				= 8;			// �ȼ�
	uint64 Exp					= 9;			// ����
	uint32 VipLevel				= 10;			// Vip�ȼ�
	uint32 VipExp				= 11;			// Vip����
}

/// ��ҵ�����Ϣ֪ͨ ID_S2C_NOTIFY_PLAYER_ITEM_INFO
message CPlayerItemInfoNotify
{
	PBItemList Bag				= 1;			// ����
}

/// ���������Ϣ֪ͨ ID_S2C_NOTIFY_PLAYER_TASK_INFO
message CPlayerTaskInfoNotify
{
	PBTaskList TaskList			= 1;			// �����б�
}

/// ��Ҽ�����Ϣ֪ͨ ID_S2C_NOTIFY_PLAYER_SKILL_INFO
message CPlayerSkillInfoNotify
{
	
}

/// �õ������������ ID_C2S_REQUEST_GET_PLAYER_PROPERTY
message CGetPlayerPropertyRequest
{

}

/// �õ�������Ի�Ӧ ID_S2C_RESPONSE_GET_PLAYER_PROPERTY
message CGetPlayerPropertyResponse
{
	repeated uint32		PropertyValue	= 1;	// ����ֵ
}

/// ����������� ID_C2S_REQUEST_HEART_BEAT
message CHeartBeatRequest
{
}

/// ���������Ӧ ID_S2C_RESPONSE_HEART_BEAT
message CHeartBeatResponse
{
	uint32	ServerTime					= 1;	// ������ʱ��
}