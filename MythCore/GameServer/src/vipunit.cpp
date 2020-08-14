#include "vipunit.h"
#include "template.h"
#include "entityplayer.h"
#include "platconfig.h"
#include "itemunit.h"
#include "errcode.h"
#include "serveractmodule.h"
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
int CVIPUnit::processRecharge(CRechargeGoods* pRechargeGoods, int nRechargeMoney)
{
	if (NULL == pRechargeGoods)
	{
		return ERR_UNKNOWN;
	}

	switch (pRechargeGoods->mType)
	{
		case emRechargeType_Diamond:
		{
			mPlayer.getItemUnit().obtainCurrency(emCurrency_Diamond, pRechargeGoods->mGameCoin);
			break;
		}
	}

	CServerActModule::Inst()->refreshProcess(emSvrActType_CumRecharge, mPlayer, pRechargeGoods->mActivityCoin, 0);
	return SUCCESS;
}
