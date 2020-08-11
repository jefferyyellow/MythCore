#include "vipunit.h"
#include "template.h"

/// 获得VIP经验
void CVIPUnit::obtainVIPExp(int nVIPExp)
{
	if (mVipLevel >= MAX_VIP_LEVEL)
	{
		return;
	}

	mVipExp += nVIPExp;
	while (mVipExp >= CTplVIPConfig::spConfig->mVIPExp[mVipLevel] && mVipLevel <= MAX_VIP_LEVEL)
	{
		mVipExp -= CTplVIPConfig::spConfig->mVIPExp[mVipLevel];
		++mVipLevel;
		if (mVipLevel == MAX_VIP_LEVEL)
		{
			mVipExp = 0;
		}
	}
}

/// 处理充值
void CVIPUnit::processRecharge(char* pGoodsID, char* pOrderID, int nRechargeMoney)
{
	if (NULL == pGoodsID || NULL == pOrderID)
	{
		return;
	}


}
