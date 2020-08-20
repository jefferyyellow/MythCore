#ifndef __ITEMSUBMODULE_H__
#define __ITEMSUBMODULE_H__
#include "playersubunit.h"
#include "itemcontainer.h"
#include "messagefactory.h"
#include "itemtype.h"
#include "equip.h"
class CEntityPlayer;
class CShopLevelData;
class CItemArrange
{
public:
	CItemArrange()
	{
		mCount = 0;
	}
	~CItemArrange()
	{}

public:
	void	addItem(int nID, int nNum);
	void	clear(){ mCount = 0; }

public:
	/// ����ID
	int		getItemID(int nIndex)
	{
		if (nIndex < 0 || nIndex >= BASE_BAG_CAPACITY)
		{
			return 0;
		}
		return mID[nIndex];
	}

	void	setItemID(int nIndex, int nID)
	{
		if (nIndex < 0 || nIndex >= BASE_BAG_CAPACITY)
		{
			return;
		}
		mID[nIndex] = nID;
	}

	int*	getIDPtr(){ return mID; }

	/// ��������
	int		getItemNum(int nIndex)
	{
		if (nIndex < 0 || nIndex >= BASE_BAG_CAPACITY)
		{
			return 0;
		}

		return mNum[nIndex];
	}

	void	setItemNum(int nIndex, int nNum)
	{
		if (nIndex < 0 || nIndex >= BASE_BAG_CAPACITY)
		{
			return;
		}
		mNum[nIndex] = nNum;
	}

	int*	getNumPtr(){ return mNum; }

	/// ���ߵȼ�
	int		getLevel(int nIndex)
	{
		if (nIndex < 0 || nIndex >= BASE_BAG_CAPACITY)
		{
			return 0;
		}

		return mLevel[nIndex];
	}

	void	setLevel(int nIndex, int nLevel)
	{
		if (nIndex < 0 || nIndex >= BASE_BAG_CAPACITY)
		{
			return;
		}

		mLevel[nIndex] = nLevel;
	}

	int*	getLevelPtr(){ return mLevel; }

	/// ��������
	int		getQuality(int nIndex)
	{
		if (nIndex < 0 || nIndex >= BASE_BAG_CAPACITY)
		{
			return 0;
		}

		return mQuality[nIndex];
	}

	void	setQuality(int nIndex, int nQuality)
	{
		if (nIndex < 0 || nIndex >= BASE_BAG_CAPACITY)
		{
			return;
		}

		mQuality[nIndex] = nQuality;
	}

	int*	getQualityPtr(){ return mQuality; }

	int GetCount() const { return mCount; }
private:
	int		mID[BASE_BAG_CAPACITY];
	int		mNum[BASE_BAG_CAPACITY];
	int		mLevel[BASE_BAG_CAPACITY];
	int		mQuality[BASE_BAG_CAPACITY];
	int		mCount;
};

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
	/// ��û���
	int			obtainCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum);
	/// ���ѻ���
	int			consumeCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum);
	/// �����Ƿ���װ����Щ���ߣ�������ҵ��ߣ�
	bool		checkItemSpace(int* pItemID, int* pItemNum, int nSize);
	/// ������ߣ�������ҵ��ߣ�
	int			insertAllItem(int* pItemID, int* pItemNum, int nSize);
	/// ӵ�е��ߵ���Ŀ��ע�⣬�������ҵ��ߣ�
	int			hasItem(int nItemID);
	/// ɾ�����ߣ�ע�⣬��������ҵ��ߣ�
	int			removeItem(int nIndex, int nItemNum);
	/// ɾ�����ߣ�ע�⣬�������ҵ��ߣ�
	void		removeItemByID(int nItemID, int nItemNum);
	/// ɾ�����ߣ�ע�⣬��������ҵ��ߣ�
	void		removeItemOnly(int nItemID, int nItemNum);
	/// �����Ƿ��ʺ����
	bool		checkMatchItem(int nItemID, int nMetier);
public:
	/// �������֪ͨ(û���������Եĵ���)
	void		sendInsertItemNotify(int nItemID, int* pIndex, int* pNumber, int nSize);
	///  �������֪ͨ(���������Եĵ���)
	void		sendInsertItemObjNotify(int nIndex);
	
	/// ʹ�õ���
	void		onUseItemRequest(Message* pMessage);
	void		sendUseItemResponse(int nResult);
	
	/// ��������
	void		onSellItemRequest(Message* pMessage);
	void		sendSellItemResponse(int nResult);

	/// ����̵������Ϣ
	void		onGetShopInfoRequest(Message* pMessage);
	void		sendGetShopInfoResponse(int nResult, CShopLevelData* pLevelData);

	/// �������
	void		onPurchaseItemRequest(Message* pMessage);
	void		sendPurchaseItemResponse(int nResult);

	/// װ������
	void		onEquipItemRequest(Message* pMessage);
	void		sendEquipItemResponse(int nResult, int nItemIndex);

	/// ж�ص���
	void		onUnEquipItemRequest(Message* pMessage);
	/// ж��װ����Ӧ
	void		sendUnEquipItemResponse(int nResult, int nEquipPart, int nItemIndex);
	/// �㲥װ���ı�֪ͨ
	void		broadcastChangeNotify(int nEntityID, int nEquipPart, int nEquipItemID);

	// ʰȡ���ߵ�����
	void		onPickItemRequest(Message* pMessage);
	// ʰȡ���ߵĻ�Ӧ
	void		sendPickItemResponse(int nResult);
public:
	/// ���õĻ��ҽӿڵ�������
	/// �õ����
	int			getMoney() const { return mCurrency[emCurrency_Money]; }
	/// ���ý�ң�һ������²����øýӿڣ�
	void		setMoney(int nMoney){mCurrency[emCurrency_Money] = nMoney;}
	/// �õ���ʯ
	int			getDiamond() const { return mCurrency[emCurrency_Diamond]; }
	/// ������ʯ��һ������²����øýӿڣ�
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
	/// �������
	int			insertItem(int nItemID, int nItemNum);

private:
	/// ����
	CItemBox				mBag;
	/// װ��
	CEquipList				mEquip;				
	/// ����
	int						mCurrency[emCurrencyMax];
};
#endif