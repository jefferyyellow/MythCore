#include "propertyunit.h"
#include "template.h"
#include "propertymodule.hxx.pb.h"
#include "rankmodule.h"
#include "entityplayer.h"
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
	while (mExp >= CTplLevelExpConfig::spConfig->mLevelUpExp[mLevel] && mLevel <= MAX_LEVEL)
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
		onLevelUp(nOldLevel);
	}
}

// 
void CPropertyUnit::onLevelUp(int nOldLevel)
{
	// 出来对应的升级的问题
	for (int i = nOldLevel + 1; i <= mLevel; ++ i)
	{

	}

	CLevelUpNotify tLevelUpNotify;
	tLevelUpNotify.set_level(mLevel);
	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_NOTIYF_LEVEL_UP, &tLevelUpNotify);

	CRankModule::Inst()->updateRoleRank(emRankType_Level, mPlayer, mLevel);
}
