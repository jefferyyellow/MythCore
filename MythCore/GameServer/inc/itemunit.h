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
	/// 得到金钱
	int			obtainMoney(int nMoney);
	/// 消费金钱
	int			consumeMoney(int nMoney);
	/// 得到钻石
	int			obtainDiamond(int nDiamond);
	/// 消费钻石
	int			consumeDiamond(int nDiamond);
	/// 包裹是否能装下这些道具
	bool		checkItemSpace(int* pItemID, int* pItemNum, int nSize);
	/// 插入道具
	int			insertItem(int* pItemID, int* pItemNum, int nSize);
	/// 插入道具
	int			insertItem(int nItemID, int nItemNum);

public:
	int		getMoney() const { return mMoney; }
	void	setMoney(int nValue) { mMoney = nValue; }

	int	getDiamond() const { return mDiamond; }
	void	setDiamond(int nValue) { mDiamond = nValue; }

	CItemBox& getBag(){ return mBag; }
private:
	/// 背包
	CItemBox				mBag;				

	/// 金钱
	int						mMoney;
	/// 钻石
	int						mDiamond;
};
#endif