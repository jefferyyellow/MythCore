require("gameserverconfig/script/error_code")

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
	ID_C2S_REQUEST_GET_PHASE_ACT_PRIZE		= 0x1C02,	-- ��ȡ�ۼƳ�ֵ����������
	ID_C2S_REQUEST_GET_PHASE_ACT_PRIZE		= 0x1C03,	-- ��ȡ�ۼƳ�ֵ�����Ļ�Ӧ
}
