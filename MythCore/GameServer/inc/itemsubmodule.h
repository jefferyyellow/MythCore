#ifndef __ITEMSUBMODULE_H__
#define __ITEMSUBMODULE_H__
#include "playersubmodule.h"
#include "itemcontainer.h"
class CEntityPlayer;
class CItemSubModule : public CPlayerSubModule<CEntityPlayer>
{
public:
	CItemSubModule(CEntityPlayer* pPlayer)
		:CPlayerSubModule(pPlayer)
	{

	}
	~CItemSubModule()
	{

	}

private:
	CItemContainer			mBag;				// ±³°ü
};
#endif