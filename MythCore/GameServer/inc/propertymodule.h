#ifndef __PROPERTYMODULE_H__
#define __PROPERTYMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
using namespace Myth;

class CEntityPlayer;
class CPropertyModule : public CLogicModule, public CSingleton < CPropertyModule >
{
	friend class CSingleton < CPropertyModule >;
private:
	CPropertyModule();
	~CPropertyModule();

public:
	/// 玩家获得经验
	void		obtainExp(CEntityPlayer* pPlayer, int nExp);
	/// 玩家等级升级
	void		onPlayerLevelUp(CEntityPlayer* pPlayer, int nLevel);

};
#endif