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

public:
	int		getMoney() const { return mMoney; }
	void	setMoney(int nValue) { mMoney = nValue; }

	int	getDiamond() const { return mDiamond; }
	void	setDiamond(int nValue) { mDiamond = nValue; }

	CItemBox& getBag(){ return mBag; }
private:
	/// ����
	CItemBox				mBag;				

	/// ��Ǯ
	int						mMoney;
	/// ��ʯ
	int						mDiamond;
};
#endif