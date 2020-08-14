require("gameserverconfig/script/server_activity/cumulative_recharge")
-- 根据不同的类型，不同的初始化函数
GlobalInitFunc ={
	CumulativeRecharge_initFunc
}

function ServerActivity_initFunc(nType)
	if nil ~= GlobalInitFunc[nType] then
		GlobalInitFunc[nType](nType)
	end
end