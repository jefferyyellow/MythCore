syntax = "proto3";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum TASK_MODULE_MSG_ID
{
	ID_TASK_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_ACCEPT_TASK				= 0x1000;	// ������������
	ID_S2C_RESPONSE_ACCEPT_TASK				= 0x1001;	// ���������Ӧ
	ID_C2S_REQUEST_SUBMIT_TASK				= 0x1002;	// ����������
	ID_S2C_RESPONSE_SUBMIT_TASK				= 0x1003;	// �������Ӧ
	ID_C2S_REQUEST_ABORT_TASK				= 0x1004;	// ������������
	ID_S2C_RESPONSE_ABORT_TASK				= 0x1005;	// ���������Ӧ
	ID_S2C_NOTIFY_UPDATE_TASK_PROCESS		= 0x1006;	// ˢ���������֪ͨ
};

// ������������ ID_C2S_REQUEST_ACCEPT_TASK
message CAcceptTaskRequest
{
		uint32	TaskID				= 1;		// ����ID
		uint32	NpcEntityID			= 2;		// NPCʵ��ID
}

// ���������Ӧ ID_S2C_RESPONSE_ACCEPT_TASK
message CAcceptTaskResponse
{
		uint32	Result				= 1;		// ���
		uint32	TaskID				= 2;		// ����ID
}

// ���������� ID_C2S_REQUEST_SUBMIT_TASK
message CSubmitTaskRequest
{
		uint32	TaskID				= 1;		// ����ID
		uint32	ItemIndex			= 2;		// ��������
}

// �������Ӧ ID_S2C_RESPONSE_SUBMIT_TASK
message CSubmitTaskResponse
{
		uint32	Result				= 1;		// ���
		uint32	TaskID				= 2;		// ����ID
}

// ������������ ID_C2S_REQUEST_ABORT_TASK
message CAbortTaskRequest
{
		uint32			TaskID		= 1;		// ����ID
}

// ���������Ӧ ID_S2C_RESPONSE_ABORT_TASK
message CAbortTaskResponse
{
		uint32			Result		= 1;		// ���
		uint32			TaskID		= 2;		// ����ID
}

// ˢ���������֪ͨ ID_S2C_NOTIFY_UPDATE_TASK_PROCESS
message CUpdateTaskProcessNotify
{
		uint32			TaskID		= 1;		// ����ID
		uint32			CondIndex	= 2;		// �����������
		uint32			Param		= 3;		// �������
}
