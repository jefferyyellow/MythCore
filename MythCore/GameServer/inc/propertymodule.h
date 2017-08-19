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
	/// ��һ�þ���
	void		obtainExp(CEntityPlayer* pPlayer, int nExp);
	/// ��ҵȼ�����
	void		onPlayerLevelUp(CEntityPlayer* pPlayer, int nLevel);

};
#endif