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
	/// ��þ���
	void obtainExp(int nExp);
	/// ���VIP����
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
	/// ����
	sint64			mExp;
	/// Vip����
	int				mVipExp;
	/// �ȼ�
	byte			mLevel;
	/// Vip�ȼ�
	byte			mVipLevel;
};
#endif