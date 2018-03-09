syntax = "proto3";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum RANK_MODULE_MSG_ID
{
	ID_RANK_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_GET_RANK_INFO			= 0x2400;	// 得到排行信息请求
	ID_S2C_RESPONSE_GET_RANK_INFO			= 0x2401;	// 得到排行信息回应
};

// 得到排行信息请求 ID_C2S_REQUEST_GET_RANK_INFO
message CGetRankInfoRequest
{
	uint32		RankType	= 1;	// 排行类型
}

// 排行榜角色信息
message PBRankRoleInfo
{
	uint32		RoleID		= 1;	// 角色ID
	uint32		RankValue	= 2;	// 排行榜值
}

// 得到排行信息回应 ID_S2C_RESPONSE_GET_RANK_INFO
message CGetRankInfoResponse
{
	uint32		RankType					= 1;	// 排行类型
	repeated	PBRankRoleInfo	RoleInfo	= 2;	// 排行榜角色信息
}

