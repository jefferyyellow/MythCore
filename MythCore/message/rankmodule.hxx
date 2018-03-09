syntax = "proto3";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum RANK_MODULE_MSG_ID
{
	ID_RANK_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_GET_RANK_INFO			= 0x2400;	// �õ�������Ϣ����
	ID_S2C_RESPONSE_GET_RANK_INFO			= 0x2401;	// �õ�������Ϣ��Ӧ
};

// �õ�������Ϣ���� ID_C2S_REQUEST_GET_RANK_INFO
message CGetRankInfoRequest
{
	uint32		RankType	= 1;	// ��������
}

// ���а��ɫ��Ϣ
message PBRankRoleInfo
{
	uint32		RoleID		= 1;	// ��ɫID
	uint32		RankValue	= 2;	// ���а�ֵ
}

// �õ�������Ϣ��Ӧ ID_S2C_RESPONSE_GET_RANK_INFO
message CGetRankInfoResponse
{
	uint32		RankType					= 1;	// ��������
	repeated	PBRankRoleInfo	RoleInfo	= 2;	// ���а��ɫ��Ϣ
}

