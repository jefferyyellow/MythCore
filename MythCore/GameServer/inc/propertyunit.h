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
		// ����
		mExp = 0;
		// �ȼ�
		mLevel = 0;
	}
	~CPropertyUnit()
	{

	}

public:
	/// �ȼ�
	uint8			getLevel() const { return mLevel; }
	void			setLevel(uint8 nValue) { mLevel = nValue; }
	
	/// ����
	uint64			getRoleExp() const { return mExp; }
	void			setRoleExp(uint64 nValue) { mExp = nValue; }

	/// VIP����
	uint32			GetVIPExp() const { return mVIPExp; }
	void			SetVIPExp(uint32 nValue) { mVIPExp = nValue; }

	/// VIP�ȼ�
	uint8	getVIPLevel() const { return mVIPLevel; }
	void	setVIPLevel(uint8 nValue) { mVIPLevel = nValue; }
private:
	/// ����
	uint64			mExp;
	/// �ȼ�
	uint8			mLevel;
	/// VIP����
	uint32			mVIPExp;
	/// VIP�ȼ�
	uint8			mVIPLevel;

};
#endif