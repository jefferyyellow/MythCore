require("gameserverconfig/script/instance/instance_type")

-- 检查玩家是否能进入该副本
function CommonInstance_checkPlayerEnter(nInstanceID, pPlayer)
	print("CommonInstance_checkPlayerEnter")
end

function CommonInstance_create(nInstanceID)
	print("CommonInstance_create")
end

function CommonInstance_end(nInstanceID)
	print("CommonInstance_end")
end

function CommonInstance_destory(nInstanceID)
	print("CommonInstance_destory")
end

function CommonInstance_givePrize(nInstanceID, pPlayer)
	print("CommonInstance_givePrize")
end

function CommonInstance_playerEnter(nInstanceID, pPlayer)
	print("CommonInstance_playerEnter")
end

function CommonInstance_onEvent(nInstanceID, nEventType, nParam1, nParam2)
	print("CommonInstance_onEvent")
end