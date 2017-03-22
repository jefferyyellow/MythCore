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
	/// 背包
	CItemContainer			mBag;				
	/// VIP等级
	uint8					mVIPLevel;
	/// 金钱
	uint32					mMoney;
	/// 钻石
	uint32					mDiamond;
};
#endif