require("gameserverconfig/script/server_activity/server_activity_type")
require("gameserverconfig/script/server_activity/cumulative_recharge")
-- 根据不同的类型，不同的初始化函数
-- 结束函数注册表，按类型
ServerActivity_GlobalEndFunc = {
	[EmSvrActType.CumRecharge] = CumulativeRecharge_end
}

-- 刷新函数注册表，按类型
ServerActivity_GlobalRefreshFunc = {
	[EmSvrActType.CumRecharge] = CumulativeRecharge_refreshProcess
}

-- 清除玩家数据函数注册表，按类型
ServerActivity_GlobalClearPlayerDataFunc = {
	[EmSvrActType.CumRecharge] = CumulativeRecharge_clearPlayerData
}

-- 得到对应活动类型的配置文件，按类型
ServerActivity_GlobalGetConfigFunc = {
	[EmSvrActType.CumRecharge] = CumulativeRecharge_getConfigName
}

-- 消息出现函数注册表，按ID
GlobalSvrActMsgFunc = {
	[0x1C02] = CumulativeRecharge_onMsgGetCumulRechargePrizeRequest
}

-- 按类型调用活动的结束函数
function ServerActivity_EndFunc(nActivityType, nActivityID)
	if nil == ServerActivity_GlobalEndFunc[nActivityType] then
		return
	end

	ServerActivity_GlobalEndFunc[nActivityType]()
end

-- 按类型调用活动的刷新函数
function ServerActivity_RefreshFunc(nActivityType, nActivityID, rPlayer, nParam1, nParam2)
	if nil == ServerActivity_GlobalRefreshFunc[nActivityType] then
		return
	end

	ServerActivity_GlobalRefreshFunc[nActivityType](nActivityID, rPlayer, nParam1, nParam2)
end

-- 按类型调用活动清除数据函数
function ServerActivity_ClearPlayerData(nActivityType, nActivityID, rPlayer)
	if nil == ServerActivity_GlobalClearPlayerDataFunc[nActivityType] then
		return
	end

	ServerActivity_GlobalClearPlayerDataFunc[nActivityType](nActivityID, rPlayer)
end


-- 按类型调用得到活动配置文件的函数
function ServerActivity_GetConfig(nActivityType)
	if nil == ServerActivity_GlobalGetConfigFunc[nActivityType] then
		return ""
	end

	return ServerActivity_GlobalGetConfigFunc[nActivityType]()
end

-- 按消息ID调用消息处理函数
function ServerActivity_onClientMessage(pPlayer, nMessageID, pMsgData)
	if GlobalSvrActMsgFunc[nMessageID] then
		GlobalSvrActMsgFunc[nMessageID](pPlayer, pMsgData)
	else
		print("Error Msg ID")
	end
end