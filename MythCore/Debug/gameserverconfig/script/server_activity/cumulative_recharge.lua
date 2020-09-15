local pb = require "pb"
assert(pb.loadfile "gameserverconfig/script/server_activity/serveractmodule.pb") 
require("gameserverconfig/script/server_activity/server_activity_type")

--require("platfile/cumulative_recharge_config")

function CumulativeRecharge_getConfigName()
	return "cumulative_recharge_config.lua"
end

function CumulativeRecharge_end()
	
end

function CumulativeRecharge_clearPlayerData(ActivityID, pPlayer)
	local pServerActUnit = pPlayer:getServerActUnit()
	-- 清除充值记录
	pServerActUnit:setActData(EmActDataType.EmCumRechargeValue, 0)
	-- 清除充值奖励
	pServerActUnit:setActData(EmActDataType.EmCumRechargePrize, 0)
end

function CumulativeRecharge_refreshProcess(ActivityID, pPlayer, nParam1, nParam2)
	local pServerActUnit = pPlayer:getServerActUnit()
	-- 累计充值金额
	local value = pServerActUnit:getActData(EmActDataType.EmCumRechargeValue) + nParam1
	pServerActUnit:setActData(EmActDataType.EmCumRechargeValue, value)
end

function CumulativeRecharge_givePlayerPrize(pPlayer, nActivityID, nIndex)
	local pServerActModule = getServerActModule()
	-- 不在奖励期间
	if not pServerActModule:checkActPrizeTime(nActivityID) then
		ServerActivity_GetPhaseActPrizeResponse(pPlayer, ERR_SERVER_ACT.ACTIVITY_IS_NOT_AVAIL)
		return 
	end

	local pServerActUnit = pPlayer:getServerActUnit()
	-- 已经领取奖励了
	if pServerActUnit:getActDataBit(EmActDataType.EmCumRechargePrize, nIndex) > 0then
		ServerActivity_GetPhaseActPrizeResponse(pPlayer, ERR_SERVER_ACT.ACTIVITY_PRIZE_ALREAD_GET)
		return
	end

	-- 活动配置不存在
	local pActivityData = CumulativeRechargeConfig[nActivityID];
	if nil == pActivityData then
		ServerActivity_GetPhaseActPrizeResponse(pPlayer, ERR_SERVER_ACT.ACTIVITY_CONFIG_IS_NOT_EXIST)
		return
	end

	-- 活动阶段数据不存在
	local nPhaseData = pActivityData.Cond[nIndex + 1]
	if nil == nPhaseData then
		ServerActivity_GetPhaseActPrizeResponse(pPlayer, ERR_SERVER_ACT.ACTIVITY_CONFIG_IS_NOT_EXIST)
		return
	end

	print("nPhaseData.Num" .. nPhaseData.Num)
	-- 充值没有达到档位
	if pServerActUnit:getActData(EmActDataType.EmCumRechargeValue) < nPhaseData.Num then
		ServerActivity_GetPhaseActPrizeResponse(pPlayer, ERR_SERVER_ACT.CUMULATIVE_RECHARGE_VALUE_NOT_ENOUGH)
		return;
	end

	-- 加入奖励列表
	local pItemArray = CItemArrange()
	for i=1,#nPhaseData.Prize do
		local prize = nPhaseData.Prize[i]
		pItemArray:addItem(prize.ItemID, prize.ItemNum)
	end

	-- 检查背包是否满足
	local pItemUnit = pPlayer:getItemUnit()
	if not pItemUnit:checkItemSpace(pItemArray:getIDPtr(), pItemArray:getNumPtr(), pItemArray:GetCount()) then
		ServerActivity_GetPhaseActPrizeResponse(pPlayer, ERR_SERVER_ACT.ERR_BAGGAGE_IS_FULL)
		return ;
	end
	-- 奖励入包
	pItemUnit:insertAllItem(pItemArray:getIDPtr(), pItemArray:getNumPtr(), pItemArray:GetCount())

	pServerActUnit:setActDataBit(EmActDataType.EmCumRechargePrize, nIndex)
	ServerActivity_GetPhaseActPrizeResponse(pPlayer, ERR_SERVER_ACT.SUCCESS)
end

