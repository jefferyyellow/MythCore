#ifndef __PROPERTYUNIT_H__
#define __PROPERTYUNIT_H__
#include "playersubunit.h"
class CEntityPlayer;
class CPropertyUnit : public CPlayerSubUnit<CEntityPlayer>
{
public:
	CPropertyUnit(CEntityPlayer* pPlayer)
	: CPlayerSubUnit(pPlayer)
	{
		// 经验
		mExp = 0;
		// 等级
		mLevel = 0;
	}
	~CPropertyUnit()
	{

	}

public:
	/// 等级
	uint8			getLevel() const { return mLevel; }
	void			setLevel(uint8 nValue) { mLevel = nValue; }
	/// 经验
	uint64			getRoleExp() const { return mExp; }
	void			setRoleExp(uint64 nValue) { mExp = nValue; }

private:
	/// 经验
	uint64			mExp;
	/// 等级
	uint8			mLevel;
};
#endif