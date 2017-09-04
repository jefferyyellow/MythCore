syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum MAP_MODULE_MSG_ID
{
	ID_MAP_MODULE_ERROR						= 0x0;
	ID_S2C_NOTIYF_ENTITY_MOVE				= 0x0C00;	// ������->�ͻ��� ʵ���ƶ�֪ͨ
	ID_S2C_NOTIYF_CREATE_PLAYER_LIST		= 0x0C01;	// ������->�ͻ��� ��������б�֪ͨ
	ID_S2C_NOTIYF_CREATE_NPC_LIST			= 0x0C02;	// ������->�ͻ��� ����NPC�б�֪ͨ
};

message CMessageEntityMoveNotify
{
	uint32	EntityID						= 1;		// ʵ���ʵ��ID
	uint32	DesPosX							= 2;		// ʵ����ƶ�����X
	uint32	DesPosY							= 3;		// ʵ����ƶ�����Y
}

message CMessagePlayerSceneInfo
{
	uint32	EntityID						= 1;		// ʵ���ʵ��ID
	uint32	PosX							= 2;		// λ��X����
	uint32	PosY							= 3;		// λ��Y����
}

message PBPlayerSceneInfo
{
	uint32	EntityID						= 1;		// ʵ���ʵ��ID
	uint32	PosX							= 2;		// λ��X����
	uint32	PosY							= 3;		// λ��Y����
}

message CMessageCreatePlayerListNotify
{
	repeated PBPlayerSceneInfo	PlayerInfo	= 1;		// �����Ϣ
}

message PBNpcSceneInfo
{
	uint32	EntityID						= 1;		// ʵ���ʵ��ID
	uint32	TempID							= 2;		// ģ��ID
	uint32	PosX							= 3;		// λ��X����
	uint32	PosY							= 4;		// λ��Y����
}

message CMessageCreateNPCListNotify
{
	repeated PBNpcSceneInfo		NPCInfo		= 1;		// NPC�б���Ϣ	
}