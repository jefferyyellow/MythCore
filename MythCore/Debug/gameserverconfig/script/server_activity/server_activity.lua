require("gameserverconfig/script/server_activity/cumulative_recharge")
-- ���ݲ�ͬ�����ͣ���ͬ�ĳ�ʼ������
GlobalInitFunc ={
	CumulativeRecharge_initFunc
}

function ServerActivity_initFunc(nType)
	if nil ~= GlobalInitFunc[nType] then
		GlobalInitFunc[nType](nType)
	end
end