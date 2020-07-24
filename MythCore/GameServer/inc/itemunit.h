#ifndef __ITEMSUBMODULE_H__
#define __ITEMSUBMODULE_H__
#include "playersubunit.h"
#include "itemcontainer.h"
#include "messagefactory.h"
#include "itemtype.h"
#include "equip.h"
class CEntityPlayer;
class CItemUnit : public CPlayerSubUnit
{
public:
	CItemUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		init();
		mBag.setSize(BASE_BAG_CAPACITY);
	}
	~CItemUnit()
	{

	}
	void init()
	{
        memset(mCurrency, 0, sizeof(mCurrency));
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
	/// 拥有道具的数目（注意，包括货币道具）
	int			hasItem(int nItemID);
	/// 删除道具（注意，不处理货币道具）
	int			removeItem(int nIndex, int nItemNum);
	/// 删除道具（注意，包括货币道具）
	void		removeItemByID(int nItemID, int nItemNum);
	/// 删除道具（注意，不处理货币道具）
	void		removeItemOnly(int nItemID, int nItemNum);

public:
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
	/// 卸载装备回应
	void		sendUnEquipItemResponse(int nResult, int nEquipPart, int nItemIndex);
	/// 广播装备改变通知
	void		broadcastChangeNotify(int nEntityID, int nEquipPart, int nEquipItemID);

	// 拾取道具的请求
	void		onPickItemRequest(Message* pMessage);
	// 拾取道具的回应
	void		sendPickItemResponse(int nResult);
public:
	/// 常用的货币接口单独出来
	/// 得到金币
	int			getMoney() const { return mCurrency[emCurrency_Money]; }
	/// 设置金币（一般情况下不能用该接口）
	void		setMoney(int nMoney){mCurrency[emCurrency_Money] = nMoney;}
	/// 得到钻石
	int			getDiamond() const { return mCurrency[emCurrency_Diamond]; }
	/// 设置钻石（一般情况下不能用该接口）
	void		setDiamond(int nDiamond){mCurrency[emCurrency_Diamond] = nDiamond;}

public:
	/// autocode don't eddit!!!
    CItemBox& getBag(){ return mBag;}

    CEquipList& getEquip(){ return mEquip;}

    int getCurrency(int nIndex)
    {
        if(nIndex < 0 || nIndex >= emCurrencyMax)
        {
            return 0;
        }
        return mCurrency[nIndex];
    }
    void setCurrency(int nIndex, int value)
    {
        if(nIndex < 0 || nIndex >= emCurrencyMax)
        {
            return;
        }
        mCurrency[nIndex] = value;
    }
	/// end autocode

private:
	/// 插入道具
	int			insertItem(int nItemID, int nItemNum);

private:
	/// 背包
	CItemBox				mBag;
	/// 装备
	CEquipList				mEquip;				
	/// 货币
	int						mCurrency[emCurrencyMax];
};
#endif