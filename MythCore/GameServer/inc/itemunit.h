#ifndef __ITEMSUBMODULE_H__
#define __ITEMSUBMODULE_H__
#include "playersubunit.h"
#include "itemcontainer.h"
#include "messagefactory.h"
#include "itemtype.h"
#define PLAYER_MONEY_LIMIT		1
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
	/// �õ���Ǯ
	int			obtainMoney(int nMoney);
	/// ���ѽ�Ǯ
	int			consumeMoney(int nMoney);
	/// �õ���ʯ
	int			obtainDiamond(int nDiamond);
	/// ������ʯ
	int			consumeDiamond(int nDiamond);
	/// �����Ƿ���װ����Щ����
	bool		checkItemSpace(int* pItemID, int* pItemNum, int nSize);
	/// �������
	int			insertItem(int* pItemID, int* pItemNum, int nSize);
	/// �������
	int			insertItem(int nItemID, int nItemNum);
	/// ӵ�е��ߵ���Ŀ
	int			hasItem(int nItemID);
	/// ɾ������
	int			removeItem(int nIndex, int nItemNum);
	/// ɾ������
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

public:
	int			getMoney() const { return mMoney; }
	void		setMoney(int nValue) { mMoney = nValue; }

	int			getDiamond() const { return mDiamond; }
	void		setDiamond(int nValue) { mDiamond = nValue; }

	CItemBox&	getBag(){ return mBag; }
private:
	/// ����
	CItemBox				mBag;				

	/// ��Ǯ
	int						mMoney;
	/// ��ʯ
	int						mDiamond;

	int						mCurrency[emCurrencyMax];
};
#endif