#include "entity.h"
#include "mapmodule.hxx.pb.h"
#include "objpool.h"
CEntity::PLAYER_ALLOC CEntity::mVisiblePlayerAlloc;
void CEntity::addVisiblePlayer(CEntity* pEntity)
{
	if (NULL == pEntity)
	{
		return;
	}

	CShareList<int>::CShareListNode<int>* pNode = mVisiblePlayerAlloc.allocate();
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

CEntity* CEntity::createEntity(EmEntityType eType)
{
	switch (eType)
	{
		case emEntityType_Player:
		{
			return (CEntity*)CObjPool::Inst()->allocObj(emObjType_Entity_Player);
			break;
		}
		case emEntityType_Ogre:
		{
			return (CEntity*)CObjPool::Inst()->allocObj(emObjType_Entity_Ogre);
			break;
		}
		case emEntityType_FuncNPC:
		{
			return (CEntity*)CObjPool::Inst()->allocObj(emObjType_Entity_FuncNPC);
			break;
		}
		case emEntityType_Item:
		{
			return (CEntity*)CObjPool::Inst()->allocObj(emObjType_Entity_Item);
			break;
		}
		default:
		{
			break;
		}
	}
	return NULL;
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


/// 刷新战斗属性
void	CEntityOgre::RefreshFightProperty()
{

}
