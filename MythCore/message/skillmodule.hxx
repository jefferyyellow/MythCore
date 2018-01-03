syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum SKILL_MODULE_MSG_ID
{
	ID_SKILL_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_USE_SKILL				= 0x1400;	// ʹ�ü�������
	ID_S2C_RESPONSE_USE_SKILL				= 0x1401;	// ʹ�ü�������
};

message CUseSkillRequest
{
				uint32	SkillIndex		= 1;		// ��������
	repeated	uint32	Target			= 2;		// Ŀ��
}

message CUseSkillResponse
{
	uint32	Result			= 1;		// ���
}