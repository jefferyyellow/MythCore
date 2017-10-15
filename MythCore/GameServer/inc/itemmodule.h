#ifndef __ITEMMODULE_H__
#define __ITEMMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"

using namespace Myth;
class CEntityPlayer;
class CItemModule : public CLogicModule, public CSingleton < CItemModule >
{
	friend class CSingleton < CItemModule >;
private:
	CItemModule();
	~CItemModule();

public:
	/// ʱ�亯��
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	void		onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);


public:
	/// �õ���Ǯ
	int			obtainMoney(CEntityPlayer* pPlayer, int nMoney);
	/// ���ѽ�Ǯ
	int			consumeMoney(CEntityPlayer* pPlayer, int nMoney);
	/// �õ���ʯ
	int			obtainDiamond(CEntityPlayer* pPlayer, int nDiamond);
	/// ������ʯ
	int			consumeDiamond(CEntityPlayer* pPlayer, int nDiamond);
	/// �����Ƿ���װ����Щ����
	bool		checkItemSpace(CEntityPlayer* pPlayer, int* pItemID, int* pItemNum,int nSize);
	/// �������
	int			insertItem(CEntityPlayer* pPlayer, int* pItemID, int* pItemNum,int nSize);
};
#endif