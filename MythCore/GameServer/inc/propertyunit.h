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
		// ����
		mExp = 0;
		// �ȼ�
		mLevel = 0;
	}
	~CPropertyUnit()
	{

	}

public:
	/// ��þ���
	void obtainExp(int nExp);
	/// ���VIP����
	void obtainVIPExp(int nVIPExp);

public:
	/// �ȼ�
	uint8			getLevel() const { return mLevel; }
	void			setLevel(uint8 nValue) { mLevel = nValue; }
	
	/// ����
	sint64			getRoleExp() const { return mExp; }
	void			setRoleExp(sint64 nValue) { mExp = nValue; }

	/// VIP����
	sint32			GetVIPExp() const { return mVIPExp; }
	void			SetVIPExp(sint32 nValue) { mVIPExp = nValue; }

	/// VIP�ȼ�
	uint8			getVIPLevel() const { return mVIPLevel; }
	void			setVIPLevel(uint8 nValue) { mVIPLevel = nValue; }
private:
	/// ����
	sint64			mExp;
	/// �ȼ�
	uint8			mLevel;
	/// VIP����
	sint32			mVIPExp;
	/// VIP�ȼ�
	uint8			mVIPLevel;

};
#endif