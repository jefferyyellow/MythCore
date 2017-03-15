#ifndef __PLAYERSUBMODULE_H__
#define __PLAYERSUBMODULE_H__
template <typename T>
class CPlayerSubModule
{
protected:
	CPlayerSubModule(T* pPlayer)
	{
		mPlayer = pPlayer;
	}
	~CPlayerSubModule()
	{
	}

public:
	T*	getPlayer(){ return mPlayer; }

protected:
	T*	mPlayer;
};
#endif