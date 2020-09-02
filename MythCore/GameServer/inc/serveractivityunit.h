#ifndef __SERVERACTIVITYUNIT_H__
#define __SERVERACTIVITYUNIT_H__
#include "serveractivitytype.h"
#include "playersubunit.h"
#include "commontype.h"
#include "bit_set.h"
using namespace Myth;
enum EmActBitType
{
	emActBit_None					= 0,	// ��
	emActBit_LevelRankPrize			= 1,	// �ȼ����а��콱
	emActBit_PowerRankPrize			= 2,	// ս�����а��콱
	// ע��ͬ��server_activity_type.lua�е�����
	emActBitMax
};

enum EmActDataType
{
	emActData_None					= 0,	// ��
	emActData_CumRechargeValue		= 1,	// �ۼƳ�ֵֵ
	emActData_CumRechargePrize		= 2,	// �ۼƳ�ֵ�콱��¼
	emActData_CumConsumeValue		= 3,	// �ۼ�����ֵ
	emActData_CumConsumePrize		= 4,	// �ۼ������콱��¼
	// ע��ͬ��server_activity_type.lua�е�����
	emActDataMax
};
/// ���Ԫ
class CServerActUnit : public CPlayerSubUnit
{
public:
	CServerActUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		memset(mActTime, 0, sizeof(mActTime));
		memset(mActData, 0, sizeof(mActData));
	}
	~CServerActUnit()
	{
		init();
	}

	void	init()
	{
       
	}

public:
	void				dailyRefresh();
	int					getActData(EmActDataType eType)
	{
		if (eType <= emActData_None || eType >= emActDataMax)
		{
			return -1;
		}

		return mActData[eType];
	}
	void				setActData(EmActDataType eType, int nData)
	{
		if (eType <= emActData_None || eType >= emActDataMax)
		{
			return ;
		}
		mActData[eType] = nData;
	}

	int					getActDataBit(EmActDataType eType, uint nBitIndex)
	{
		if (eType <= emActData_None || eType >= emActDataMax || nBitIndex >= sizeof(int) * 8)
		{
			return 0;
		}

		return mActData[eType] & (1 << nBitIndex);
	}
	void				setActDataBit(EmActDataType eType, uint nBitIndex)
	{
		if (eType <= emActData_None || eType >= emActDataMax || nBitIndex >= sizeof(int) * 8)
		{
			return;
		}
		mActData[eType] |= (1 << nBitIndex);
	}

	void				clearActDataBit(EmActDataType eType, uint nBitIndex)
	{
		if (eType <= emActData_None || eType >= emActDataMax || nBitIndex >= sizeof(int) * 8)
		{
			return;
		}
		mActData[eType] &= ~(1 << nBitIndex);
	}

	bool				getActBit(uint nBitIndex)
	{
		return mActBit.getBit(nBitIndex);
	}

	void				setActBit(uint nBitIndex)
	{
		mActBit.setBit(nBitIndex);
	}
	
	void				clearBit(uint nBitIndex)
	{
		mActBit.clearBit(nBitIndex);
	}
	// autocode
	// end autocode
private:
	/// ��¼ÿ����Ŀ�ʼʱ��
	time_t					mActTime[MAX_SERVER_ACT_NUM];
	/// ����ݼ�¼
	int						mActData[emActDataMax];
	/// �λ���ݼ�¼
	CBitSet<emActBitMax>	mActBit;
};
#endif