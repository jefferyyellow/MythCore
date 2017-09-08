#include "entity.h"
#include "mapmodule.hxx.pb.h"
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


/// 序列化场景信息到PB·
void CEntityNPC::serializeSceneInfoToPB(PBNpcSceneInfo* pbNpcInfo)
{
	if (NULL == pbNpcInfo)
	{
		return;
	}
	pbNpcInfo->set_entityid(getObjID());
	pbNpcInfo->set_entityid(mTempID);
	pbNpcInfo->set_posx(mPos.mX);
	pbNpcInfo->set_posy(mPos.mY);
}
