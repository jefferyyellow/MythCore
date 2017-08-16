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
		mMoney			= 0;
		mDiamond		= 0;
	}
	~CItemUnit()
	{

	}

public:
	uint32	getMoney() const { return mMoney; }
	void	setMoney(uint32 nValue) { mMoney = nValue; }

	uint32	getDiamond() const { return mDiamond; }
	void	setDiamond(uint32 nValue) { mDiamond = nValue; }

private:
	/// ±³°ü
	CItemContainer			mBag;				
	/// ½ðÇ®
	uint32					mMoney;
	/// ×êÊ¯
	uint32					mDiamond;
};
#endif