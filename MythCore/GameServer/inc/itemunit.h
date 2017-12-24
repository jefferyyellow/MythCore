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
		memset(mCurrency, 0, sizeof(mCurrency));
	}
	~CItemUnit()
	{

	}

public:
	/// ��û���
	int			obtainCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum);
	/// ���ѻ���
	int			consumeCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum);
	/// �����Ƿ���װ����Щ���ߣ�������ҵ��ߣ�
	bool		checkItemSpace(int* pItemID, int* pItemNum, int nSize);
	/// ������ߣ�������ҵ��ߣ�
	int			insertItem(int* pItemID, int* pItemNum, int nSize);
	/// ӵ�е��ߵ���Ŀ��ע�⣬���������ҵ��ߣ�
	int			hasItem(int nItemID);
	/// ɾ�����ߣ�ע�⣬��������ҵ��ߣ�
	int			removeItem(int nIndex, int nItemNum);
	/// ɾ�����ߣ�ע�⣬��������ҵ��ߣ�
	void		removeItemByID(int nItemID, int nItemNum);

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

	CItemBox&	getBag(){ return mBag; }
	CEquipList& getEquipList(){return mEquip;}
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