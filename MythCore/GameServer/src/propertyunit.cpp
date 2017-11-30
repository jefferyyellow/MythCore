#include "propertyunit.h"
#include "template.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"
/// 获得经验
void CPropertyUnit::obtainExp(int nExp)
{
	if (nExp <= 0)
	{
		return;
	}
	// 最大等级了，不能再升级了
	if (mLevel >= MAX_LEVEL)
	{
		return;
	}

	mExp += nExp;

	byte nOldLevel = mLevel;
	while (mLevel >= CTplLevelExpConfig::spConfig->mLevelUpExp[mLevel] && mLevel <= MAX_LEVEL)
	{
		mExp -= CTplLevelExpConfig::spConfig->mLevelUpExp[mLevel];
		++mLevel;
		// 已经到达最大级了
		if (mLevel >= MAX_LEVEL)
		{
			mExp = 0;
			break;
		}
	}

	if (mLevel > nOldLevel)
	{
		CLevelUpNotify tLevelUpNotify;
		tLevelUpNotify.set_level(mLevel);
		CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_LEVEL_UP, &tLevelUpNotify);
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