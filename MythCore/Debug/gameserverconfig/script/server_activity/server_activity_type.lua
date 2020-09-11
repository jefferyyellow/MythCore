require("gameserverconfig/script/error_code")

EmActDataType =
{
	EmActDataNone			= 0,	-- 无
	EmCumRechargeValue		= 1,	-- 累计充值值
	EmCumRechargePrize		= 2,	-- 累计充值领奖记录
	EmCumConsumeValue		= 3,	-- 累计消费值
	EmCumConsumePrize		= 4,	-- 累计消费领奖记录
}

EmActBitType = 
{
	EmActBitNone			= 0,	-- 无
	EmLevelRankPrize		= 1,	-- 等级排行榜领奖
	EmPowerRankPrize		= 2,	-- 战力排行榜领奖
}

-- 开服活动类型
EmSvrActType = 
{
	None					= 0,	-- 无类型
	CumRecharge				= 1,	-- 累计充值
	CumConsume				= 2,	-- 累计消费
	PlayerRank				= 3,	-- 排行活动
	ServerActTypeMax		= 4,	-- 需要加类型的话在这个前面加
}

EmSvrActMsg =
{
	ID_C2S_REQUEST_GET_SERVER_ACT			= 0x1C00,	-- 得到开服活动的请求
	ID_S2C_RESPONSE_GET_SERVER_ACT			= 0x1C01,	-- 得到开服活动的回应
	ID_C2S_REQUEST_GET_PHASE_ACT_PRIZE		= 0x1C02,	-- 领取累计充值奖励的请求
	ID_C2S_REQUEST_GET_PHASE_ACT_PRIZE		= 0x1C03,	-- 领取累计充值奖励的回应
}
