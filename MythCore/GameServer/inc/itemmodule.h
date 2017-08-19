#ifndef __ITEMMODULE_H__
#define __ITEMMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
using namespace Myth;
class CEntityPlayer;
class CItemModule : public CLogicModule, public CSingleton < CItemModule >
{
	friend class CSingleton < CItemModule >;
private:
	CItemModule();
	~CItemModule();

public:
	/// �õ���Ǯ
	int			obtainMoney(CEntityPlayer* pPlayer, int nMoney);
	/// ���ѽ�Ǯ
	int			consumeMoney(CEntityPlayer* pPlayer, int nMoney);
	/// �õ���ʯ
	int			obtainDiamond(CEntityPlayer* pPlayer, int nDiamond);
	/// ������ʯ
	int			consumeDiamond(CEntityPlayer* pPlayer, int nDiamond);

};
#endif