syntax = "proto3";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum TASK_MODULE_MSG_ID
{
	ID_TASK_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_ACCEPT_TASK				= 0x1000;	// ������������
	ID_S2C_RESPONSE_ACCEPT_TASK				= 0x1001;	// ���������Ӧ
	ID_C2S_REQUEST_SUBMIT_TASK				= 0x1002;	// ����������
	ID_S2C_RESPONSE_SUBMIT_TASK				= 0x1003;	// �������Ӧ
};

// ������������ ID_C2S_REQUEST_ACCEPT_TASK
message CAcceptTaskRequest
{
		uint32	TaskID	= 1;		// ����ID
}

// ���������Ӧ ID_S2C_RESPONSE_ACCEPT_TASK
message CAcceptTaskResponse
{
		uint32	Result	= 1;		// ���
		uint32	TaskID	= 2;		// ����ID
}

// ���������� ID_C2S_REQUEST_SUBMIT_TASK
message CSubmitTaskRequest
{
		uint32	TaskID	= 1;		// ����ID
}

// �������Ӧ ID_S2C_RESPONSE_SUBMIT_TASK
message CSubmitTaskResponse
{
		uint32	Result	= 1;		// ���
}