syntax = "proto3";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum TASK_MODULE_MSG_ID
{
	ID_TASK_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_ACCEPT_TASK				= 0x1000;	// 接受任务请求
	ID_S2C_RESPONSE_ACCEPT_TASK				= 0x1001;	// 接受任务回应
	ID_C2S_REQUEST_SUBMIT_TASK				= 0x1002;	// 交任务请求
	ID_S2C_RESPONSE_SUBMIT_TASK				= 0x1003;	// 交任务回应
	ID_C2S_REQUEST_ABORT_TASK				= 0x1004;	// 放弃任务请求
	ID_S2C_RESPONSE_ABORT_TASK				= 0x1005;	// 放弃任务回应
	ID_S2C_NOTIFY_UPDATE_TASK_PROCESS		= 0x1006;	// 刷新任务进度通知
};

// 接受任务请求 ID_C2S_REQUEST_ACCEPT_TASK
message CAcceptTaskRequest
{
		uint32	TaskID				= 1;		// 任务ID
		uint32	NpcEntityID			= 2;		// NPC实体ID
}

// 接受任务回应 ID_S2C_RESPONSE_ACCEPT_TASK
message CAcceptTaskResponse
{
		uint32	Result				= 1;		// 结果
		uint32	TaskID				= 2;		// 任务ID
}

// 交任务请求 ID_C2S_REQUEST_SUBMIT_TASK
message CSubmitTaskRequest
{
		uint32	TaskID				= 1;		// 任务ID
		uint32	ItemIndex			= 2;		// 道具索引
}

// 交任务回应 ID_S2C_RESPONSE_SUBMIT_TASK
message CSubmitTaskResponse
{
		uint32	Result				= 1;		// 结果
		uint32	TaskID				= 2;		// 任务ID
}

// 放弃任务请求 ID_C2S_REQUEST_ABORT_TASK
message CAbortTaskRequest
{
		uint32			TaskID		= 1;		// 任务ID
}

// 放弃任务回应 ID_S2C_RESPONSE_ABORT_TASK
message CAbortTaskResponse
{
		uint32			Result		= 1;		// 结果
		uint32			TaskID		= 2;		// 任务ID
}

// 刷新任务进度通知 ID_S2C_NOTIFY_UPDATE_TASK_PROCESS
message CUpdateTaskProcessNotify
{
		uint32			TaskID		= 1;		// 任务ID
		uint32			CondIndex	= 2;		// 完成条件索引
		uint32			Param		= 3;		// 任务参数
}
