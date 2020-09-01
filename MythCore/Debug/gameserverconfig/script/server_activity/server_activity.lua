require("gameserverconfig/script/server_activity/server_activity_type")
require("gameserverconfig/script/server_activity/cumulative_recharge")
-- ���ݲ�ͬ�����ͣ���ͬ�ĳ�ʼ������
-- ��������ע���������
ServerActivity_GlobalEndFunc = {
	[EmSvrActType.CumRecharge] = CumulativeRecharge_end
}

-- ˢ�º���ע���������
ServerActivity_GlobalRefreshFunc = {
	[EmSvrActType.CumRecharge] = CumulativeRecharge_refreshProcess
}

-- ���������ݺ���ע���������
ServerActivity_GlobalClearPlayerDataFunc = {
	[EmSvrActType.CumRecharge] = CumulativeRecharge_clearPlayerData
}

-- �õ���Ӧ����͵������ļ���������
ServerActivity_GlobalGetConfigFunc = {
	[EmSvrActType.CumRecharge] = CumulativeRecharge_getConfigName
}

-- ��Ϣ���ֺ���ע�����ID
GlobalSvrActMsgFunc = {
	[0x1C02] = CumulativeRecharge_onMsgGetCumulRechargePrizeRequest
}

-- �����͵��û�Ľ�������
function ServerActivity_EndFunc(nActivityType, nActivityID)
	if nil == ServerActivity_GlobalEndFunc[nActivityType] then
		return
	end

	ServerActivity_GlobalEndFunc[nActivityType]()
end

-- �����͵��û��ˢ�º���
function ServerActivity_RefreshFunc(nActivityType, nActivityID, rPlayer, nParam1, nParam2)
	if nil == ServerActivity_GlobalRefreshFunc[nActivityType] then
		return
	end

	ServerActivity_GlobalRefreshFunc[nActivityType](nActivityID, rPlayer, nParam1, nParam2)
end

-- �����͵��û������ݺ���
function ServerActivity_ClearPlayerData(nActivityType, nActivityID, rPlayer)
	if nil == ServerActivity_GlobalClearPlayerDataFunc[nActivityType] then
		return
	end

	ServerActivity_GlobalClearPlayerDataFunc[nActivityType](nActivityID, rPlayer)
end


-- �����͵��õõ�������ļ��ĺ���
function ServerActivity_GetConfig(nActivityType)
	if nil == ServerActivity_GlobalGetConfigFunc[nActivityType] then
		return ""
	end

	return ServerActivity_GlobalGetConfigFunc[nActivityType]()
end

-- ����ϢID������Ϣ������
function ServerActivity_onClientMessage(pPlayer, nMessageID, pMsgData)
	if GlobalSvrActMsgFunc[nMessageID] then
		GlobalSvrActMsgFunc[nMessageID](pPlayer, pMsgData)
	else
		print("Error Msg ID")
	end
end