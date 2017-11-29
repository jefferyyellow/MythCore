syntax = "proto3";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum TASK_MODULE_MSG_ID
{
	ID_TASK_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_ACCEPT_TASK				= 0x1000;	// 接受任务请求
	ID_S2C_RESPONSE_ACCEPT_TASK				= 0x1001;	// 接受任务回应
	ID_C2S_REQUEST_SUBMIT_TASK				= 0x1002;	// 交任务请求
	ID_S2C_RESPONSE_SUBMIT_TASK				= 0x1003;	// 交任务回应
};

// 接受任务请求 ID_C2S_REQUEST_ACCEPT_TASK
message CAcceptTaskRequest
{
		uint32	TaskID	= 1;		// 任务ID
}

// 接受任务回应 ID_S2C_RESPONSE_ACCEPT_TASK
message CAcceptTaskResponse
{
		uint32	Result	= 1;		// 结果
		uint32	TaskID	= 2;		// 任务ID
}

// 交任务请求 ID_C2S_REQUEST_SUBMIT_TASK
message CSubmitTaskRequest
{
		uint32	TaskID	= 1;		// 任务ID
}

// 交任务回应 ID_S2C_RESPONSE_SUBMIT_TASK
message CSubmitTaskResponse
{
		uint32	Result	= 1;		// 结果
}