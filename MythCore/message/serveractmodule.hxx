syntax = "proto3";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum SERVER_ACT_MODULE_MSG_ID
{
	ID_SERVER_ACT_MODULE_ERROR				= 0x0;
	ID_C2S_REQUEST_GET_SERVER_ACT			= 0x1C00;	// 得到开服活动的请求
	ID_S2C_RESPONSE_GET_SERVER_ACT			= 0x1C01;	// 得到开服活动的回应
	ID_C2S_REQUEST_GET_PHASE_ACT_PRIZE		= 0x1C02;	// 领取阶段性活动奖励请求
	ID_S2C_RESPONSE_GET_PHASE_ACT_PRIZE		= 0x1C03;	// 领取阶段性活动奖励回应
};

// 得到开服活动的请求 ID_C2S_REQUEST_GET_SERVER_ACT
message CGetServerActRequest
{
}

// 得到开服活动的回应 ID_S2C_RESPONSE_GET_SERVER_ACT
message CGetServerActResponse
{

}

// 领取阶段性活动奖励请求 ID_C2S_REQUEST_GET_PHASE_ACT_PRIZE
message CGetPhaseActPrizeRequest
{
	uint32		ActivityID		= 1;		// 活动ID
	uint32		Index			= 2;		// 奖励索引
}

// 领取阶段性活动奖励回应 ID_S2C_RESPONSE_GET_PHASE_ACT_PRIZE
message CGetPhaseActPrizeResponse
{
	uint32		Result			= 1;		// 结果
}