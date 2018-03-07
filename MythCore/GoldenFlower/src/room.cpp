#include "room.h"

#include "entityplayer.h"
void CRoom::init()
{
	for (int i = 0; i < MAX_GOLD_FLOUR_POKER_NUM; ++ i)
	{
		mPoker[i] = i;
	}
	mPokerNum = MAX_GOLD_FLOUR_POKER_NUM;
}

void CRoom::addPlayer(CEntityPlayer* pPlayer)
{
	if (mPlayerNum >= MAX_GOLD_FLOUR_PLAYER_NUM)
	{
		return;
	}

	mPlayer[mPlayerNum] = pPlayer->getRoleID();
	++ mPlayerNum;
}

int CRoom::randomDrawPoker()
{
	if (mPokerNum <= 0)
	{
		return -1;
	}
	int nRand = RAND(mPokerNum);
	
	int nPoker = mPoker[nRand];
	mPoker[nRand] = mPoker[mPokerNum - 1];
	return nPoker;
}
