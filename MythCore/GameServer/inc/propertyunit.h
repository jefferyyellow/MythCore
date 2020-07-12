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
	}

public:
	/// ��þ���
	void obtainExp(int nExp);
	void onLevelUp(int nOldLevel);

public:
	/// autocode, don't edit
    sint64 getExp(){ return mExp;}
    void setExp(sint64 value){ mExp = value;}

    byte getLevel(){ return mLevel;}
    void setLevel(byte value){ mLevel = value;}
	/// end autocode

private:
	/// ����
	sint64			mExp;
	/// �ȼ�
	byte			mLevel;
};
#endif