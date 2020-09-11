require("gameserverconfig/script/instance/instance_type")
require("gameserverconfig/script/instance/common_instance")


AllInstConfig={};

-- 消息出现函数注册表，按ID
GlobalInstanceMsgFunc = {
	
}

-- 按消息ID调用消息处理函数
function InstanceModule_onClientMessage(pPlayer, nMessageID, pMsgData)
	if GlobalInstanceMsgFunc[nMessageID] then
		GlobalInstanceMsgFunc[nMessageID](pPlayer, pMsgData)
	else
		print("Error Msg ID")
	end
end


-- 根据不同的类型，不同的初始化函数

-- 检查玩家是否能进副本,按类型
Instance_GlobalCheckPlayerEnterFunc = {
	[EmInstanceType.Common] = CommonInstance_checkPlayerEnter
}

-- 创建函数注册表，按类型
Instance_GlobalCreateFunc = {
	[EmInstanceType.Common] = CommonInstance_create
}

-- 给玩家进入副本的特殊处理，按类型
Instance_GlobalPlayerEnterFunc = {
	[EmInstanceType.Common] = CommonInstance_playerEnter
}

-- 结束函数注册表，按类型
Instance_GlobalEndFunc = {
	[EmInstanceType.Common] = CommonInstance_end
}

-- 销毁函数注册表，按类型
Instance_GlobalDestroyFunc = {
	[EmInstanceType.Common] = CommonInstance_destory
}

-- 给玩家奖励的函数注册表，按类型
Instance_GlobalGivePrizeFunc = {
	[EmInstanceType.Common] = CommonInstance_givePrize
}

-- 副本事件，按类型
Instance_GlobalOnEventFunc = {
	[EmInstanceType.Common] = CommonInstance_onEvent
}


-- 按类型调用副本的检查玩家是否能进入函数
function Instance_CheckPlayerEnterFunc(nInstanceType, nInstanceID, pPlayer)
	-- 统一的副本检查
	if pPlayer:getLevel() < AllInstConfig[nInstanceID].PlayerLevel then
		return -100012
	end

	-- 单独类型的副本检查
	if nil == Instance_GlobalCheckPlayerEnterFunc[nInstanceType] then
		return -1
	end
	return Instance_GlobalCheckPlayerEnterFunc[nInstanceType](nInstanceID, pPlayer)
end

-- 按类型调用副本的创建函数
function Instance_CreateFunc(nInstanceType, nInstanceID)
	if nil == Instance_GlobalCreateFunc[nInstanceType] then
		return
	end

	Instance_GlobalCreateFunc[nInstanceType](nInstanceID)
end

-- 按类型调用玩家进入的函数
function ServerActivity_PlayerEnterFunc(nActivityType, nActivityID, pPlayer)
	if nil == Instance_GlobalPlayerEnterFunc[nActivityType] then
		return ""
	end

	return Instance_GlobalPlayerEnterFunc[nActivityType](nActivityID, pPlayer)
end

-- 按类型调用副本的结束函数
function Instance_EndFunc(nInstanceType, nInstanceID)
	if nil == Instance_GlobalEndFunc[nInstanceType] then
		return
	end

	Instance_GlobalEndFunc[nInstanceType](nInstanceID)
end

-- 按类型调用副本的销毁函数
function Instance_DestroyFunc(nActivityType, nActivityID)
	if nil == Instance_GlobalDestroyFunc[nActivityType] then
		return
	end

	Instance_GlobalDestroyFunc[nActivityType](nActivityID)
end


-- 按类型调用给玩家奖励的函数
function ServerActivity_GivePrizeFunc(nActivityType, nActivityID, pPlayer)
	if nil == Instance_GlobalGivePrizeFunc[nActivityType] then
		return ""
	end

	return Instance_GlobalGivePrizeFunc[nActivityType](nActivityID, pPlayer)
end

-- 按类型调用副本事件的函数
function ServerActivity_OnEventFunc(nActivityType, nActivityID, nEventType, nParam1, nParam2)
	if nil == Instance_GlobalOnEventFunc[nActivityType] then
		return ""
	end

	return Instance_GlobalOnEventFunc[nActivityType](nActivityID, nEventType, nParam1, nParam2)
end

-- 初始化副本的lua配置文件,CInstanceModule::loadAllInstConfig中调用
function initInstLuaConfig(nInstanceID)
	AllInstConfig[nInstanceID] = InstanceConfig;
end