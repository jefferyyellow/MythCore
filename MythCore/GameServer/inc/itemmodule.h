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
	/// 时间函数
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	void		onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);


public:
	/// 得到金钱
	int			obtainMoney(CEntityPlayer* pPlayer, int nMoney);
	/// 消费金钱
	int			consumeMoney(CEntityPlayer* pPlayer, int nMoney);
	/// 得到钻石
	int			obtainDiamond(CEntityPlayer* pPlayer, int nDiamond);
	/// 消费钻石
	int			consumeDiamond(CEntityPlayer* pPlayer, int nDiamond);
	/// 包裹是否能装下这些道具
	bool		checkItemSpace(CEntityPlayer* pPlayer, int* pItemID, int* pItemNum,int nSize);
	/// 插入道具
	int			insertItem(CEntityPlayer* pPlayer, int* pItemID, int* pItemNum,int nSize);
};
#endif