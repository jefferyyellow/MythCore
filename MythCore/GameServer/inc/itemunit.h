#ifndef __ITEMSUBMODULE_H__
#define __ITEMSUBMODULE_H__
#include "playersubunit.h"
#include "itemcontainer.h"
#include "messagefactory.h"
#include "itemtype.h"
class CEntityPlayer;
class CItemUnit : public CPlayerSubUnit
{
public:
	CItemUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		mMoney			= 0;
		mDiamond		= 0;
	}
	~CItemUnit()
	{

	}

public:
	/// 获得货币
	int			obtainCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum);
	/// 消费货币
	int			consumeCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum);
	/// 包裹是否能装下这些道具
	bool		checkItemSpace(int* pItemID, int* pItemNum, int nSize);
	/// 插入道具
	int			insertItem(int* pItemID, int* pItemNum, int nSize);
	/// 插入道具
	int			insertItem(int nItemID, int nItemNum);
	/// 拥有道具的数目
	int			hasItem(int nItemID);
	/// 删除道具
	int			removeItem(int nIndex, int nItemNum);
	/// 删除道具
	void		removeItemByID(int nItemID, int nItemNum);


	/// 插入道具通知(没有特殊属性的道具)
	void		sendInsertItemNotify(int nItemID, int* pIndex, int* pNumber, int nSize);
	///  插入道具通知(有特殊属性的道具)
	void		sendInsertItemObjNotify(int nIndex);
	
	/// 使用道具
	void		onUseItemRequest(Message* pMessage);
	void		sendUseItemResponse(int nResult);
	
	/// 卖出道具
	void		onSellItemRequest(Message* pMessage);
	void		sendSellItemResponse(int nResult);

	/// 购买道具
	void		onPurchaseItemRequest(Message* pMessage);
	void		sendPurchaseItemResponse(int nResult);

public:
	int			getMoney() const { return mMoney; }
	void		setMoney(int nValue) { mMoney = nValue; }

	int			getDiamond() const { return mDiamond; }
	void		setDiamond(int nValue) { mDiamond = nValue; }

	CItemBox&	getBag(){ return mBag; }
private:
	/// 背包
	CItemBox				mBag;				

	/// 金钱
	int						mMoney;
	/// 钻石
	int						mDiamond;

	int						mCurrency[emCurrencyMax];
};
#endif