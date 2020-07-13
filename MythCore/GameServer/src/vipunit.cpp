#include "vipunit.h"
#include "template.h"

/// ���VIP����
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

void processRecharge(uint nAccountID, short nChannelID, short nServerID, uint nRoleID,
	char* pGoodsID, char* pOrderID, double dRechargeMoney)
{

}