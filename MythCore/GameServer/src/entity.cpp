#include "entity.h"
CEntity::PLAYER_ALLOC CEntity::mVisiblePlayerAlloc;
void CEntity::addVisiblePlayer(CEntity* pEntity)
{
	if (NULL == pEntity)
	{
		return;
	}

	CShareList<uint32>::CShareListNode<uint32>* pNode = mVisiblePlayerAlloc.allocate();
	pNode->mData = pEntity->getObjID();
	pNode->mpNext = NULL;
	pNode->mpPrev = NULL;
	mVisiblePlayer.push_front(pNode);
}

void CEntity::removeVisiblePlayer(CEntity* pEntity)
{
	if (NULL == pEntity)
	{
		return;
	}
	PLAYER_LIST::iterator it = mVisiblePlayer.begin();
	for (; it != mVisiblePlayer.end(); ++it)
	{
		if (*it == pEntity->getObjID())
		{
			mVisiblePlayer.eraseNode(it);
			mVisiblePlayerAlloc.free(it.mNode);
			break;
		}
	}
}