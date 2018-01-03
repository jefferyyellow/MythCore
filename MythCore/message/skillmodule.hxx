syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum SKILL_MODULE_MSG_ID
{
	ID_SKILL_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_USE_SKILL				= 0x1400;	// 使用技能请求
	ID_S2C_RESPONSE_USE_SKILL				= 0x1401;	// 使用技能请求
};

message CUseSkillRequest
{
				uint32	SkillIndex		= 1;		// 技能索引
	repeated	uint32	Target			= 2;		// 目标
}

message CUseSkillResponse
{
	uint32	Result			= 1;		// 结果
}