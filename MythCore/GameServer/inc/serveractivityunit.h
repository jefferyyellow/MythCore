#ifndef __SERVERACTIVITYUNIT_H__
#define __SERVERACTIVITYUNIT_H__
#include "serveractivitytype.h"
#include "playersubunit.h"
#include "commontype.h"
#include "bit_set.h"
using namespace Myth;
enum EmActBitType
{
	emActBit_None					= 0,	// 无
	emActBit_LevelRankPrize			= 1,	// 等级排行榜领奖
	emActBit_PowerRankPrize			= 2,	// 战力排行榜领奖
	// 注意同步server_activity_type.lua中的类型
	emActBitMax
};

enum EmActDataType
{
	emActData_None					= 0,	// 无
	emActData_CumRechargeValue		= 1,	// 累计充值值
	emActData_CumRechargePrize		= 2,	// 累计充值领奖记录
	emActData_CumConsumeValue		= 3,	// 累计消费值
	emActData_CumConsumePrize		= 4,	// 累计消费领奖记录
	// 注意同步server_activity_type.lua中的类型
	emActDataMax
};
/// 活动单元
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
	/// 记录每个活动的开始时间
	time_t					mActTime[MAX_SERVER_ACT_NUM];
	/// 活动数据记录
	int						mActData[emActDataMax];
	/// 活动位数据记录
	CBitSet<emActBitMax>	mActBit;
};
#endif