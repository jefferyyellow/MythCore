#include "entityplayer.h"
void CEntityPlayer::addPoker(byte nPoker)
{
	if (mPokerNum >= MAX_PLAYER_POKER_NUM)
	{
		return;
	}
	mPoker[mPokerNum] = nPoker;
	++ nPoker;
}

void CEntityPlayer::clear()
{
	mPokerNum = 0;
}