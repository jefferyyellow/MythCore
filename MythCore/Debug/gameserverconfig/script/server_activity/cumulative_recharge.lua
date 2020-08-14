
require("gameserverconfig/script/server_activity/server_type")

function CumulativeRecharge_initFunc(nType)
	pServerActInst = getServerActModuleInst()
	pServerActInst:initActivityTypeFunc(nType, 1, "CumulativeRecharge_end")
	pServerActInst:initActivityTypeFunc(nType, 2, "CumulativeRecharge_clearPlayerData")
	pServerActInst:initActivityTypeFunc(nType, 3, "CumulativeRecharge_refreshProcess")
end

function CumulativeRecharge_end()
	
end

function CumulativeRecharge_clearPlayerData(ActivityID, pPlayer)
	pServerActUnit = pPlayer:getServerActUnit()
	-- 清除充值记录
	pServerActUnit:setActData(EmActDataType.EmCumRechargeValue, 0)
	-- 清除充值奖励
	pServerActUnit:setActData(EmActDataType.EmCumRechargePrize, 0)
end

function CumulativeRecharge_refreshProcess(ActivityID, pPlayer, nParam1, nParam2)
	pServerActUnit = pPlayer:getServerActUnit()
	-- 累计充值金额
	value = pServerActUnit:getActData(EmActDataType.EmCumRechargeValue) + nParam1
	pServerActUnit:setActData(EmActDataType.EmCumRechargeValue, value)
end