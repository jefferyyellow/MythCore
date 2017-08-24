#include "propertyunit.h"
#include "template.h"
/// ��þ���
void CPropertyUnit::obtainExp(int nExp)
{
	// ���ȼ��ˣ�������������
	if (mLevel >= MAX_LEVEL)
	{
		return;
	}

	mExp += nExp;

	while (mLevel >= CTplPlayerLevelUpExpConfig::spConfig->mLevelUpExp[mLevel] && mLevel <= MAX_LEVEL)
	{
		mExp -= CTplPlayerLevelUpExpConfig::spConfig->mLevelUpExp[mLevel];
		++mLevel;
		// �Ѿ����������
		if (mLevel >= MAX_LEVEL)
		{
			mExp = 0;
			break;
		}
	}
}

/// ���VIP����
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