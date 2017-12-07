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
	/// ɾ�����ߣ�ע�⣬���������ҵ��ߣ�
	int			removeItem(int nIndex, int nItemNum);
	/// ɾ�����ߣ�ע�⣬���������ҵ��ߣ�
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
	void		sendUnEquipItemResponse(int nResult, int nEquipIndex, int nItemIndex);

public:
	/// ���õĻ��ҽӿڵ�������
	/// �õ����
	int			getMoney() const { return mCurrency[emCurrency_Money]; }
	/// �õ���ʯ
	int			getDiamond() const { return mCurrency[emCurrency_Diamond]; }

	CItemBox&	getBag(){ return mBag; }
private:
	/// �������
	int			insertItem(int nItemID, int nItemNum);

private:
	/// ����
	CItemBox				mBag;				
	/// ����
	int						mCurrency[emCurrencyMax];
};
#endif