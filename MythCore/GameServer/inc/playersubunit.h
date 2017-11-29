#ifndef __PLAYERSUBMODULE_H__
#define __PLAYERSUBMODULE_H__
class CEntityPlayer;
class CPlayerSubUnit
{
protected:
	CPlayerSubUnit(CEntityPlayer& rPlayer)
		:mPlayer(rPlayer)
	{
	}
	~CPlayerSubUnit()
	{
	}

public:
	CEntityPlayer& getPlayer(){ return mPlayer; }

protected:
	CEntityPlayer&	mPlayer;
};
#endif