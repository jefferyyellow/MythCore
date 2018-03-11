#ifndef __ROOM_H__
#define __ROOM_H__
#define MAX_GOLD_FLOUR_PLAYER_NUM			5
#define MIN_GOLD_FLOUR_PLAYER_NUM			3
#define GOLD_FLOUR_POKER_NUM				3		// 诈金花牌的数量
#define MAX_GOLD_FLOUR_POKER_NUM			52
#include "obj.h"
class CEntityPlayer;
class CRoom : public CObj
{
public:
	CRoom() 
	{
		mPokerNum = 0;
	}
	~CRoom()
	{
	}

	void	init();
	int		randomDrawPoker();
	void	addPlayer(CEntityPlayer* pPlayer);
	void	removePlayer(CEntityPlayer* pPlayer);
	void	onPlayerReady(CEntityPlayer* pPlayer);
	
private:
	int		mPlayer[MAX_GOLD_FLOUR_PLAYER_NUM];
	int		mPoker[MAX_GOLD_FLOUR_POKER_NUM];
	int		mPokerNum;
};
#endif