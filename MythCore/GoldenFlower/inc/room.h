#ifndef __ROOM_H__
#define __ROOM_H__
#define MAX_GOLD_FLOUR_PLAYER_NUM			5
#define MAX_GOLD_FLOUR_POKER_NUM			52
#include "obj.h"
class CEntityPlayer;
class CRoom : CObj
{
public:
	CRoom()
	{
		mPlayerNum = 0;
		mPokerNum = 0;
	}
	~CRoom()
	{
	}

	void	init();
	int		randomDrawPoker();
	void	addPlayer(CEntityPlayer* pPlayer);
	
private:
	int		mPlayer[MAX_GOLD_FLOUR_PLAYER_NUM];
	int		mPlayerNum;
	int		mPoker[MAX_GOLD_FLOUR_POKER_NUM];
	int		mPokerNum;
};
#endif