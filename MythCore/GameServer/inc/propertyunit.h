#ifndef __PROPERTYUNIT_H__
#define __PROPERTYUNIT_H__
#include "playersubunit.h"
#include "servercommon.h"
class CEntityPlayer;
class CPropertyUnit : public CPlayerSubUnit
{
public:
	CPropertyUnit(CEntityPlayer& rPlayer)
	: CPlayerSubUnit(rPlayer)
	{

	}
	~CPropertyUnit()
	{

	}

	void init()
	{
        mExp = 0;
        mLevel = 0;
        mVipExp = 0;
        mVipLevel = 0;
	}

public:
	/// 获得经验
	void obtainExp(int nExp);
	/// 获得VIP经验
	void obtainVIPExp(int nVIPExp);

public:
	/// autocode, don't edit
    sint64 getExp(){ return mExp;}
    void setExp(sint64 value){ mExp = value;}

    byte getLevel(){ return mLevel;}
    void setLevel(byte value){ mLevel = value;}

    int getVipExp(){ return mVipExp;}
    void setVipExp(int value){ mVipExp = value;}

    byte getVipLevel(){ return mVipLevel;}
    void setVipLevel(byte value){ mVipLevel = value;}
	/// end autocode
private:
	/// 经验
	sint64			mExp;
	/// Vip经验
	int				mVipExp;
	/// 等级
	byte			mLevel;
	/// Vip等级
	byte			mVipLevel;
};
#endif