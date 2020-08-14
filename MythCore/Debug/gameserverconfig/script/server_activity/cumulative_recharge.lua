
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
	-- �����ֵ��¼
	pServerActUnit:setActData(EmActDataType.EmCumRechargeValue, 0)
	-- �����ֵ����
	pServerActUnit:setActData(EmActDataType.EmCumRechargePrize, 0)
end

function CumulativeRecharge_refreshProcess(ActivityID, pPlayer, nParam1, nParam2)
	pServerActUnit = pPlayer:getServerActUnit()
	-- �ۼƳ�ֵ���
	value = pServerActUnit:getActData(EmActDataType.EmCumRechargeValue) + nParam1
	pServerActUnit:setActData(EmActDataType.EmCumRechargeValue, value)
end