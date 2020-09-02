-- 800 - 1099

ERR_COMMON = 
{
	SUCCESS										= 0,		-- 成功
	ERR_UNKNOWN									= 300,		-- 未知错误
	ERR_PARAMETER_INVALID						= 301,		-- 参数非法
	ERR_CURRENCY_NOT_ENOUGH						= 302,		-- 货币不足
	ERR_TEMPLATE_INVALID						= 303,		-- 模板非法
	ERR_BAGGAGE_IS_FULL							= 304,		-- 包裹已满
}

 ERR_SERVER_ACT = 
 {
	-- Lua新定义的
	ACTIVITY_IS_NOT_AVAIL						= 800,		-- 活动没有开启
	ACTIVITY_PRIZE_ALREAD_GET					= 801,		-- 活动奖励已经领取
	ACTIVITY_CONFIG_IS_NOT_EXIST				= 802,		-- 活动配置不存在
	CUMULATIVE_RECHARGE_VALUE_NOT_ENOUGH		= 803,		-- 累计充值不够
 }
