syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum MAP_MODULE_MSG_ID
{
	ID_MAP_MODULE_ERROR						= 0x0;
	ID_S2C_NOTIYF_ENTITY_MOVE				= 0x0C00;	// ʵ���ƶ�֪ͨ
	ID_S2C_NOTIYF_CREATE_PLAYER_LIST		= 0x0C01;	// ��������б�֪ͨ
	ID_S2C_NOTIYF_CREATE_NPC_LIST			= 0x0C02;	// ����NPC�б�֪ͨ
	ID_S2C_NOTIYF_DESTROY_ENTITY			= 0x0C03;	// ����ʵ��֪ͨ
	ID_C2S_REQUEST_PLAYER_MOVE				= 0x0C04;	// ����ƶ�������
	ID_S2C_RESPONSE_PLAYER_MOVE				= 0x0C05;	// ����ƶ���Ӧ
	ID_C2S_REQUEST_PLAYER_TELEPORT			= 0x0C06;	// ��Ҵ�������
	ID_S2C_RESPONSE_PLAYER_TELEPORT			= 0x0C07;	// ��Ҵ��ͻ�Ӧ
};

// ʵ���ƶ�֪ͨ ID_S2C_NOTIYF_ENTITY_MOVE
message CEntityMoveNotify
{
	uint32	EntityID						= 1;		// ʵ���ʵ��ID
	uint32	DesPosX							= 2;		// ʵ����ƶ�����X
	uint32	DesPosY							= 3;		// ʵ����ƶ�����Y
}

// ��ҳ�����Ϣ
message PBPlayerSceneInfo
{
	uint32	EntityID						= 1;		// ʵ���ʵ��ID
	uint32	PosX							= 2;		// λ��X����
	uint32	PosY							= 3;		// λ��Y����
}

// ��������б�֪ͨ ID_S2C_NOTIYF_CREATE_PLAYER_LIST
message CCreatePlayerListNotify
{
	repeated PBPlayerSceneInfo	PlayerInfo	= 1;		// �����Ϣ
}

// NPC������Ϣ
message PBNpcSceneInfo
{
	uint32	EntityID						= 1;		// ʵ���ʵ��ID
	uint32	TempID							= 2;		// ģ��ID
	uint32	PosX							= 3;		// λ��X����
	uint32	PosY							= 4;		// λ��Y����
}

// ����NPC�б�֪ͨ ID_S2C_NOTIYF_CREATE_NPC_LIST
message CCreateNPCListNotify
{
	repeated PBNpcSceneInfo		NPCInfo		= 1;		// NPC�б���Ϣ	
}

// �Ƴ�ʵ��֪ͨ ID_S2C_NOTIYF_DESTROY_ENTITY
message CDestroyEntityNotify
{
	repeated uint32				EntityID	= 1;		// ʵ���б�
}

// ����ƶ�����	ID_C2S_REQUEST_PLAYER_MOVE
message CPlayerMoveRequest
{
	uint32	DesPosX							= 1;		// Ŀ��λ��X����
	uint32	DesPosY							= 2;		// Ŀ��λ��Y����
}

// ����ƶ���Ӧ ID_S2C_RESPONSE_PLAYER_MOVE
message CPlayerMoveResponse
{
	uint32	Result							= 1;		// ���
}

// ��Ҵ������� ID_C2S_REQUEST_PLAYER_TELEPORT
message CPlayerTeleportRequest
{
	uint32	MapID							= 1;		// ����ȥ�ĵ�ͼ
}

// ��Ҵ��ͻ�Ӧ ID_S2C_RESPONSE_PLAYER_TELEPORT
message CPlayerTeleportResponse
{
	uint32	Result							= 1;		// ���
}