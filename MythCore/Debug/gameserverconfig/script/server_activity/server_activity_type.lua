EmActDataType =
{
	EmActDataNone			= 0,	-- ��
	EmCumRechargeValue		= 1,	-- �ۼƳ�ֵֵ
	EmCumRechargePrize		= 2,	-- �ۼƳ�ֵ�콱��¼
	EmCumConsumeValue		= 3,	-- �ۼ�����ֵ
	EmCumConsumePrize		= 4,	-- �ۼ������콱��¼
}

EmActBitType = 
{
	EmActBitNone			= 0,	-- ��
	EmLevelRankPrize		= 1,	-- �ȼ����а��콱
	EmPowerRankPrize		= 2,	-- ս�����а��콱
}

-- ���������
EmSvrActType = 
{
	None					= 0,	-- ������
	CumRecharge				= 1,	-- �ۼƳ�ֵ
	CumConsume				= 2,	-- �ۼ�����
	PlayerRank				= 3,	-- ���л
	ServerActTypeMax		= 4,	-- ��Ҫ�����͵Ļ������ǰ���
}

EmSvrActMsg =
{
	ID_C2S_REQUEST_GET_SERVER_ACT			= 0x1C00,	-- �õ������������
	ID_S2C_RESPONSE_GET_SERVER_ACT			= 0x1C01,	-- �õ�������Ļ�Ӧ
	ID_C2S_REQUEST_GET_CUMUL_RECHARGE_PRIZE	= 0x1C02,	-- ��ȡ�ۼƳ�ֵ����������
	ID_S2C_RESPONSE_GET_CUMUL_RECHARGE_PRIZE= 0x1C03,	-- ��ȡ�ۼƳ�ֵ�����Ļ�Ӧ
}

-- 800 - 1099
 ERR_SERVER_ACT = 
 {
	-- C++ �Ѿ������
	SUCCESS										= 0,		-- �ɹ�
	ERR_BAGGAGE_IS_FULL							= 304,		-- ��������
	-- Lua�¶����
	ACTIVITY_IS_NOT_AVAIL						= 800,		-- �û�п���
	ACTIVITY_PRIZE_ALREAD_GET					= 801,		-- ������Ѿ���ȡ
	ACTIVITY_CONFIG_IS_NOT_EXIST				= 802,		-- ����ò�����
	CUMULATIVE_RECHARGE_VALUE_NOT_ENOUGH		= 803,		-- �ۼƳ�ֵ����
 }
