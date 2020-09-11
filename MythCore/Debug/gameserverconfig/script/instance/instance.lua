require("gameserverconfig/script/instance/instance_type")
require("gameserverconfig/script/instance/common_instance")


AllInstConfig={};

-- ��Ϣ���ֺ���ע�����ID
GlobalInstanceMsgFunc = {
	
}

-- ����ϢID������Ϣ������
function InstanceModule_onClientMessage(pPlayer, nMessageID, pMsgData)
	if GlobalInstanceMsgFunc[nMessageID] then
		GlobalInstanceMsgFunc[nMessageID](pPlayer, pMsgData)
	else
		print("Error Msg ID")
	end
end


-- ���ݲ�ͬ�����ͣ���ͬ�ĳ�ʼ������

-- �������Ƿ��ܽ�����,������
Instance_GlobalCheckPlayerEnterFunc = {
	[EmInstanceType.Common] = CommonInstance_checkPlayerEnter
}

-- ��������ע���������
Instance_GlobalCreateFunc = {
	[EmInstanceType.Common] = CommonInstance_create
}

-- ����ҽ��븱�������⴦��������
Instance_GlobalPlayerEnterFunc = {
	[EmInstanceType.Common] = CommonInstance_playerEnter
}

-- ��������ע���������
Instance_GlobalEndFunc = {
	[EmInstanceType.Common] = CommonInstance_end
}

-- ���ٺ���ע���������
Instance_GlobalDestroyFunc = {
	[EmInstanceType.Common] = CommonInstance_destory
}

-- ����ҽ����ĺ���ע���������
Instance_GlobalGivePrizeFunc = {
	[EmInstanceType.Common] = CommonInstance_givePrize
}

-- �����¼���������
Instance_GlobalOnEventFunc = {
	[EmInstanceType.Common] = CommonInstance_onEvent
}


-- �����͵��ø����ļ������Ƿ��ܽ��뺯��
function Instance_CheckPlayerEnterFunc(nInstanceType, nInstanceID, pPlayer)
	-- ͳһ�ĸ������
	if pPlayer:getLevel() < AllInstConfig[nInstanceID].PlayerLevel then
		return -100012
	end

	-- �������͵ĸ������
	if nil == Instance_GlobalCheckPlayerEnterFunc[nInstanceType] then
		return -1
	end
	return Instance_GlobalCheckPlayerEnterFunc[nInstanceType](nInstanceID, pPlayer)
end

-- �����͵��ø����Ĵ�������
function Instance_CreateFunc(nInstanceType, nInstanceID)
	if nil == Instance_GlobalCreateFunc[nInstanceType] then
		return
	end

	Instance_GlobalCreateFunc[nInstanceType](nInstanceID)
end

-- �����͵�����ҽ���ĺ���
function ServerActivity_PlayerEnterFunc(nActivityType, nActivityID, pPlayer)
	if nil == Instance_GlobalPlayerEnterFunc[nActivityType] then
		return ""
	end

	return Instance_GlobalPlayerEnterFunc[nActivityType](nActivityID, pPlayer)
end

-- �����͵��ø����Ľ�������
function Instance_EndFunc(nInstanceType, nInstanceID)
	if nil == Instance_GlobalEndFunc[nInstanceType] then
		return
	end

	Instance_GlobalEndFunc[nInstanceType](nInstanceID)
end

-- �����͵��ø��������ٺ���
function Instance_DestroyFunc(nActivityType, nActivityID)
	if nil == Instance_GlobalDestroyFunc[nActivityType] then
		return
	end

	Instance_GlobalDestroyFunc[nActivityType](nActivityID)
end


-- �����͵��ø���ҽ����ĺ���
function ServerActivity_GivePrizeFunc(nActivityType, nActivityID, pPlayer)
	if nil == Instance_GlobalGivePrizeFunc[nActivityType] then
		return ""
	end

	return Instance_GlobalGivePrizeFunc[nActivityType](nActivityID, pPlayer)
end

-- �����͵��ø����¼��ĺ���
function ServerActivity_OnEventFunc(nActivityType, nActivityID, nEventType, nParam1, nParam2)
	if nil == Instance_GlobalOnEventFunc[nActivityType] then
		return ""
	end

	return Instance_GlobalOnEventFunc[nActivityType](nActivityID, nEventType, nParam1, nParam2)
end

-- ��ʼ��������lua�����ļ�,CInstanceModule::loadAllInstConfig�е���
function initInstLuaConfig(nInstanceID)
	AllInstConfig[nInstanceID] = InstanceConfig;
end