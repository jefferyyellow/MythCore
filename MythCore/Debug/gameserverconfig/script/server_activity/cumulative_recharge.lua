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
	-- �����ֵ��¼
	pServerActUnit:setActData(EmActDataType.EmCumRechargeValue, 0)
	-- �����ֵ����
	pServerActUnit:setActData(EmActDataType.EmCumRechargePrize, 0)
end

function CumulativeRecharge_refreshProcess(ActivityID, pPlayer, nParam1, nParam2)
	local pServerActUnit = pPlayer:getServerActUnit()
	-- �ۼƳ�ֵ���
	local value = pServerActUnit:getActData(EmActDataType.EmCumRechargeValue) + nParam1
	pServerActUnit:setActData(EmActDataType.EmCumRechargeValue, value)
end

function CumulativeRecharge_onMsgGetCumulRechargePrizeRequest(pPlayer, pMsgData)
	local msg = assert(pb.decode("CGetCumulRechargePrizeRequest", pMsgData))
	local nActivityID = msg.ActivityID
	local nIndex = msg.Index
	
	local pServerActModule = getServerActModule()
	-- ���ڽ����ڼ�
	if not pServerActModule:checkActPrizeTime(nActivityID) then
		sendGetCumulRechargePrizeResponse(pPlayer, ERR_SERVER_ACT.ACTIVITY_IS_NOT_AVAIL)
		return 
	end

	local pServerActUnit = pPlayer:getServerActUnit()
	-- �Ѿ���ȡ������
	if pServerActUnit:getActDataBit(EmActDataType.EmCumRechargePrize, nIndex) > 0then
		sendGetCumulRechargePrizeResponse(pPlayer, ERR_SERVER_ACT.ACTIVITY_PRIZE_ALREAD_GET)
		return
	end

	-- ����ò�����
	local pActivityData = CumulativeRechargeConfig[nActivityID];
	if nil == pActivityData then
		sendGetCumulRechargePrizeResponse(pPlayer, ERR_SERVER_ACT.ACTIVITY_CONFIG_IS_NOT_EXIST)
		return
	end

	-- ��׶����ݲ�����
	local nPhaseData = pActivityData.Cond[nIndex + 1]
	if nil == nPhaseData then
		sendGetCumulRechargePrizeResponse(pPlayer, ERR_SERVER_ACT.ACTIVITY_CONFIG_IS_NOT_EXIST)
		return
	end

	print("nPhaseData.Num" .. nPhaseData.Num)
	-- ��ֵû�дﵽ��λ
	if pServerActUnit:getActData(EmActDataType.EmCumRechargeValue) < nPhaseData.Num then
		sendGetCumulRechargePrizeResponse(pPlayer, ERR_SERVER_ACT.CUMULATIVE_RECHARGE_VALUE_NOT_ENOUGH)
		return;
	end

	-- ���뽱���б�
	local pItemArray = CItemArrange()
	for i=1,#nPhaseData.Prize do
		local prize = nPhaseData.Prize[i]
		pItemArray:addItem(prize.ItemID, prize.ItemNum)
	end

	-- ��鱳���Ƿ�����
	local pItemUnit = pPlayer:getItemUnit()
	if not pItemUnit:checkItemSpace(pItemArray:getIDPtr(), pItemArray:getNumPtr(), pItemArray:GetCount()) then
		sendGetCumulRechargePrizeResponse(pPlayer, ERR_SERVER_ACT.ERR_BAGGAGE_IS_FULL)
		return ;
	end
	-- �������
	pItemUnit:insertAllItem(pItemArray:getIDPtr(), pItemArray:getNumPtr(), pItemArray:GetCount())

	pServerActUnit:setActDataBit(EmActDataType.EmCumRechargePrize, nIndex)
	sendGetCumulRechargePrizeResponse(pPlayer, ERR_SERVER_ACT.SUCCESS)
end

function sendGetCumulRechargePrizeResponse(pPlayer, nResult)
	local Response = {
		Result = nResult;
	}

	print("sendGetCumulRechargePrizeResponse")
	local bytes = assert(pb.encode("CGetCumulRechargePrizeResponse", Response));
	local pSceneJob = getSceneJob();
	pSceneJob:send2Player(pPlayer, EmSvrActMsg.ID_S2C_RESPONSE_GET_CUMUL_RECHARGE_PRIZE, bytes, string.len(bytes))
end