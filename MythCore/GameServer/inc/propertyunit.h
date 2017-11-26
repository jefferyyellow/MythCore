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
	byte			getLevel() const { return mLevel; }
	void			setLevel(byte nValue) { mLevel = nValue; }
	
	/// ����
	sint64			getRoleExp() const { return mExp; }
	void			setRoleExp(sint64 nValue) { mExp = nValue; }

	/// VIP����
	int				GetVIPExp() const { return mVIPExp; }
	void			SetVIPExp(int nValue) { mVIPExp = nValue; }

	/// VIP�ȼ�
	byte			getVIPLevel() const { return mVIPLevel; }
	void			setVIPLevel(byte nValue) { mVIPLevel = nValue; }
private:
	/// ����
	sint64			mExp;
	/// �ȼ�
	byte			mLevel;
	/// VIP����
	int				mVIPExp;
	/// VIP�ȼ�
	byte			mVIPLevel;

};
#endif