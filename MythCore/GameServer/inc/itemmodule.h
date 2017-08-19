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
	/// 得到金钱
	int			obtainMoney(CEntityPlayer* pPlayer, int nMoney);
	/// 消费金钱
	int			consumeMoney(CEntityPlayer* pPlayer, int nMoney);
	/// 得到钻石
	int			obtainDiamond(CEntityPlayer* pPlayer, int nDiamond);
	/// 消费钻石
	int			consumeDiamond(CEntityPlayer* pPlayer, int nDiamond);

};
#endif