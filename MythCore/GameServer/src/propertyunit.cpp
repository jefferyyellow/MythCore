#include "propertyunit.h"
#include "template.h"
/// 获得经验
void CPropertyUnit::obtainExp(int nExp)
{
	// 最大等级了，不能再升级了
	if (mLevel >= MAX_LEVEL)
	{
		return;
	}

	mExp += nExp;

	while (mLevel >= CTplPlayerLevelUpExpConfig::spConfig->mLevelUpExp[mLevel] && mLevel <= MAX_LEVEL)
	{
		mExp -= CTplPlayerLevelUpExpConfig::spConfig->mLevelUpExp[mLevel];
		++mLevel;
		// 已经到达最大级了
		if (mLevel >= MAX_LEVEL)
		{
			mExp = 0;
			break;
		}
	}
}

/// 获得VIP经验
void CPropertyUnit::obtainVIPExp(int nVIPExp)
{
	if (mVIPLevel >= MAX_VIP_LEVEL)
	{
		return;
	}

	mVIPExp += nVIPExp;
	while (mVIPExp >= CTplVIPConfig::spConfig->mVIPExp[mVIPLevel] && mVIPLevel <= MAX_VIP_LEVEL)
	{
		mVIPExp -= CTplVIPConfig::spConfig->mVIPExp[mVIPLevel];
		++mVIPLevel;
	}

	if (mVIPLevel == MAX_VIP_LEVEL)
	{
		mVIPExp = 0;
	}
}