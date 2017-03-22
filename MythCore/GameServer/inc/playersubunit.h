#ifndef __PLAYERSUBMODULE_H__
#define __PLAYERSUBMODULE_H__
template <typename T>
class CPlayerSubUnit
{
protected:
	CPlayerSubUnit(T* pPlayer)
	{
		mPlayer = pPlayer;
	}
	~CPlayerSubUnit()
	{
	}

public:
	T*	getPlayer(){ return mPlayer; }

protected:
	T*	mPlayer;
};
#endif