#ifndef __TASKUNIT_H__
#define __TASKUNIT_H__
#include "playersubunit.h"
class CEntityPlayer;
class CTaskUnit : public CPlayerSubUnit < CEntityPlayer >
{
public:
	CTaskUnit(CEntityPlayer* pPlayer)
		: CPlayerSubUnit(pPlayer)
	{

	}
	~CTaskUnit()
	{

	}
};
#endif