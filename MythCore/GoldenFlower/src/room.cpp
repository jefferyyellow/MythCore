#include "room.h"

#include "entityplayer.h"
#include "objpool.h"
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
	// 是否已经在里面了
	for (int i = 0; i < MAX_GOLD_FLOUR_PLAYER_NUM; ++i)
	{
		if (pPlayer->getObjID() == mPlayer[i])
		{
			return;
		}
	}
	for (int i = 0; i < MAX_GOLD_FLOUR_PLAYER_NUM; ++i)
	{
		if (INVALID_OBJ_ID == mPlayer[i])
		{
			mPlayer[i] = 0;
			break;
		}
	}

}

void CRoom::removePlayer(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	for (int i = 0; i < MAX_GOLD_FLOUR_PLAYER_NUM; ++ i)
	{
		if (pPlayer->getObjID() == mPlayer[i])
		{
			mPlayer[i] = INVALID_OBJ_ID;
			break;
		}
	}
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

void CRoom::onPlayerReady(CEntityPlayer* pPlayer)
{
	int nCount = 0;
	for (int i = 0; i < MAX_GOLD_FLOUR_PLAYER_NUM; ++i)
	{
		if (INVALID_OBJ_ID != mPlayer[i])
		{
			CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(mPlayer[i]));
			if (NULL != pPlayer)
			{
				++ nCount;
			}
		}
	}
	// 大于最少人数
	if (nCount >= MIN_GOLD_FLOUR_PLAYER_NUM)
	{

	}
}

