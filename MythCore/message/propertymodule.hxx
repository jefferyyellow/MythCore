syntax = "proto3";

// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum PROPERTY_MODULE_MSG_ID
{
	ID_PROPERTY_MODULE_ERROR			= 0x0;
	ID_S2C_NOTIYF_LEVEL_UP				= 0x0400;		// ������->�ͻ��� �������֪ͨ
	ID_C2S_REQUEST_GM_COMMAND			= 0x0401;		// �ͻ���->������ ���GM��������
	ID_S2C_RESPONSE_GM_COMMAND			= 0x0402;		// ������->�ͻ��� ���GM�����Ӧ
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