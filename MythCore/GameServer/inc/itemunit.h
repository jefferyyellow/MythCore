#ifndef __ITEMSUBMODULE_H__
#define __ITEMSUBMODULE_H__
#include "playersubunit.h"
#include "itemcontainer.h"

#define PLAYER_MONEY_LIMIT		1
class CEntityPlayer;
class CItemUnit : public CPlayerSubUnit<CEntityPlayer>
{
public:
	CItemUnit(CEntityPlayer* pPlayer)
		:CPlayerSubUnit(pPlayer)
	{
		mMoney			= 0;
		mDiamond		= 0;
	}
	~CItemUnit()
	{

	}

public:
	sint32	getMoney() const { return mMoney; }
	void	setMoney(sint32 nValue) { mMoney = nValue; }

	sint32	getDiamond() const { return mDiamond; }
	void	setDiamond(sint32 nValue) { mDiamond = nValue; }

private:
	/// ±³°ü
	CItemContainer			mBag;				
	/// ½ðÇ®
	sint32					mMoney;
	/// ×êÊ¯
	sint32					mDiamond;
};
#endif