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
		memset(mCurrency, 0, sizeof(mCurrency));
	}
	~CItemUnit()
	{

	}

public:
	/// 获得货币
	int			obtainCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum);
	/// 消费货币
	int			consumeCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum);
	/// 包裹是否能装下这些道具（处理货币道具）
	bool		checkItemSpace(int* pItemID, int* pItemNum, int nSize);
	/// 插入道具（处理货币道具）
	int			insertItem(int* pItemID, int* pItemNum, int nSize);
	/// 拥有道具的数目（注意，不出来货币道具）
	int			hasItem(int nItemID);
	/// 删除道具（注意，不出来货币道具）
	int			removeItem(int nIndex, int nItemNum);
	/// 删除道具（注意，不出来货币道具）
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

	/// 装备道具
	void		onEquipItemRequest(Message* pMessage);
	void		sendEquipItemResponse(int nResult, int nItemIndex);

	/// 卸载道具
	void		onUnEquipItemRequest(Message* pMessage);
	void		sendUnEquipItemResponse(int nResult, int nEquipIndex, int nItemIndex);

public:
	/// 常用的货币接口单独出来
	/// 得到金币
	int			getMoney() const { return mCurrency[emCurrency_Money]; }
	/// 得到钻石
	int			getDiamond() const { return mCurrency[emCurrency_Diamond]; }

	CItemBox&	getBag(){ return mBag; }
private:
	/// 插入道具
	int			insertItem(int nItemID, int nItemNum);

private:
	/// 背包
	CItemBox				mBag;				
	/// 货币
	int						mCurrency[emCurrencyMax];
};
#endif