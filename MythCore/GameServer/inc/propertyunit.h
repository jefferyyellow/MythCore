#ifndef __PROPERTYUNIT_H__
#define __PROPERTYUNIT_H__
#include "playersubunit.h"
#include "commontype.h"
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
	/// 获得经验
	void obtainExp(int nExp);
	/// 获得VIP经验
	void obtainVIPExp(int nVIPExp);

public:
	/// 等级
	byte			getLevel() const { return mLevel; }
	void			setLevel(byte nValue) { mLevel = nValue; }
	
	/// 经验
	sint64			getRoleExp() const { return mExp; }
	void			setRoleExp(sint64 nValue) { mExp = nValue; }

	/// VIP经验
	int				GetVIPExp() const { return mVIPExp; }
	void			SetVIPExp(int nValue) { mVIPExp = nValue; }

	/// VIP等级
	byte			getVIPLevel() const { return mVIPLevel; }
	void			setVIPLevel(byte nValue) { mVIPLevel = nValue; }
private:
	/// 经验
	sint64			mExp;
	/// 等级
	byte			mLevel;
	/// VIP经验
	int				mVIPExp;
	/// VIP等级
	byte			mVIPLevel;

};
#endif