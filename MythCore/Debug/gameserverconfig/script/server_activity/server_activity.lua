local pb = require "pb"
assert(pb.loadfile "gameserverconfig/script/server_activity/serveractmodule.pb")

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

-- ��ý�������ע���������
ServerActivity_GivePlayerPrizeFunc = {
	[EmSvrActType.CumRecharge] = CumulativeRecharge_givePlayerPrize
}


-- �����͵��û�Ľ�������
function ServerActivity_EndFunc(nActivityType, nActivityID)
	if nil == ServerActivity_GlobalEndFunc[nActivityType] then
		return
	end

	ServerActivity_GlobalEndFunc[nActivityType]()
end

-- �����͵��û��ˢ�º���
function ServerActivity_RefreshFunc(nActivityType, nActivityID, pPlayer, nParam1, nParam2)
	if nil == ServerActivity_GlobalRefreshFunc[nActivityType] then
		return
	end

	ServerActivity_GlobalRefreshFunc[nActivityType](nActivityID, pPlayer, nParam1, nParam2)
end

-- �����͵��û������ݺ���
function ServerActivity_ClearPlayerData(nActivityType, nActivityID, pPlayer)
	if nil == ServerActivity_GlobalClearPlayerDataFunc[nActivityType] then
		return
	end

	ServerActivity_GlobalClearPlayerDataFunc[nActivityType](nActivityID, pPlayer)
end


-- �����͵��õõ�������ļ����ֵĺ���
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
		print("Error Msg ID" .. nMessageID)
	end
end

function ServerActivity_GetPhaseActPrizeRequest(pPlayer, pMsgData)
	local msg = assert(pb.decode("CGetPhaseActPrizeRequest", pMsgData))
	local nActivityID = msg.ActivityID
	local nIndex = msg.Index

	local pServerActModule = getServerActModule()
	-- ���ڽ����ڼ�
	local nActivityType = pServerActModule:getActType(nActivityID)

	if nil == ServerActivity_GivePlayerPrizeFunc[nActivityType] then
		return
	end

	ServerActivity_GivePlayerPrizeFunc[nActivityType](pPlayer, nActivityID, nIndex)
end

function ServerActivity_GetPhaseActPrizeResponse(pPlayer, nResult)
	local Response = {
		Result = nResult;
	}

	print("ServerActivity_GetPhaseActPrizeResponse")
	local bytes = assert(pb.encode("CGetPhaseActPrizeResponse", Response));
	local pSceneJob = getSceneJob();
	pSceneJob:send2Player(pPlayer, EmSvrActMsg.ID_S2C_RESPONSE_GET_PHASE_ACT_PRIZE, bytes, string.len(bytes))
end

-- ��Ϣ���ֺ���ע�����ID
GlobalSvrActMsgFunc = {
	[EmSvrActMsg.ID_C2S_REQUEST_GET_PHASE_ACT_PRIZE] = ServerActivity_GetPhaseActPrizeRequest
}