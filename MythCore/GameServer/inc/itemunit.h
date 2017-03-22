#ifndef __ITEMSUBMODULE_H__
#define __ITEMSUBMODULE_H__
#include "playersubunit.h"
#include "itemcontainer.h"
class CEntityPlayer;
class CItemUnit : public CPlayerSubUnit<CEntityPlayer>
{
public:
	CItemUnit(CEntityPlayer* pPlayer)
		:CPlayerSubUnit(pPlayer)
	{
		mVIPLevel		= 0;
		mMoney			= 0;
		mDiamond		= 0;
	}
	~CItemUnit()
	{

	}

public:
	uint8	getVIPLevel() const { return mVIPLevel; }
	void	setVIPLevel(uint8 nValue) { mVIPLevel = nValue; }

	uint32	getMoney() const { return mMoney; }
	void	setMoney(uint32 nValue) { mMoney = nValue; }

	uint32	getDiamond() const { return mDiamond; }
	void	setDiamond(uint32 nValue) { mDiamond = nValue; }

private:
	/// ����
	CItemContainer			mBag;				
	/// VIP�ȼ�
	uint8					mVIPLevel;
	/// ��Ǯ
	uint32					mMoney;
	/// ��ʯ
	uint32					mDiamond;
};
#endif