#include "entity.h"
#include "mapmodule.hxx.pb.h"
#include "objpool.h"
#include "entityplayer.h"
#include "itemmodule.h"
#include "mapmodule.h"
#include "entitycreator.h"
CEntity::PLAYER_ALLOC CEntity::mVisiblePlayerAlloc;
void CEntity::init()
{
    mTempID = 0;
    mLineID = 0;
    mMapID = 0;
    mMapIndex = 0;
    mEntityType = emEntityType_None;
}
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
			return static_cast<CEntity*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
			break;
		}
		case emEntityType_Ogre:
		{
			return static_cast<CEntity*>(CObjPool::Inst()->allocObj(emObjType_Entity_Ogre));
			break;
		}
		case emEntityType_FuncNPC:
		{
			return static_cast<CEntity*>(CObjPool::Inst()->allocObj(emObjType_Entity_FuncNPC));
			break;
		}
		case emEntityType_Item:
		{
			return static_cast<CEntity*>(CObjPool::Inst()->allocObj(emObjType_Entity_Item));
			break;
		}
		default:
		{
			break;
		}
	}
	return NULL;
}

void CEntity::destroyEntity(CEntity* pEntity)
{
	if (NULL == pEntity)
	{
		return;
	}

	CObjPool::Inst()->free(pEntity->getObjID());
}

void CEntityCharacter::init()
{
    memset(mFightProperty, 0, sizeof(mFightProperty));
    mCurHP = 0;
    mCurMP = 0;
}

void CEntityCharacter::reduceHp(CEntityCharacter* pSrcEntity, int nNum)
{
	if (mCurHP <= 0)
	{
		return;
	}

	mCurHP -= nNum;
	if (mCurHP <= 0)
	{
		onDie(pSrcEntity);
	}
}

void CEntityCharacter::addHp(CEntityCharacter* pSrcEntity, int nNum)
{
	if (mCurHP <= 0)
	{
		return;
	}

	mCurHP += nNum;
}

void CEntityCharacter::onDie(CEntityCharacter* pKiller)
{
	// 删除死亡应该去掉的BUFF
	// 设置死亡状态
	// 死亡计时器处理

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
void CEntityOgre::refreshFightProperty()
{

}

/// 初始化
void CEntityOgre::initEntity(CEntityCreator* pCreator)
{
	
}

// 暂时没有考虑组队的问题，实现一个简单的
void CEntityOgre::onDead(CEntityCharacter* pKiller)
{
	if (NULL == pKiller || !(pKiller->isPlayer()))
	{
		return;
	}

	CTplOgre* pTplOgre = static_cast<CTplOgre*>(CStaticData::searchTpl(getTempID()));
	if (NULL == pTplOgre)
	{
		return;
	}


	// 杀怪经验
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(pKiller);
	deadDrop(pPlayer, pTplOgre);

	
	
	// 怪物掉落

	// 任务的杀怪计数
	pPlayer->getTaskUnit().refreshTask(emComplete_KillOgre, getTempID(), 1);
}


/// 死亡掉落
void CEntityOgre::deadDrop(CEntityPlayer* pPlayer, CTplOgre* pTplOgre)
{
	if (NULL == pPlayer || NULL == pTplOgre)
	{
		return;
	}
	pPlayer->getPropertyUnit().obtainExp(pTplOgre->mExp);

	int nItemNum = 0;
	for (int i = 0; i < MAX_OGRE_DROP; ++ i)
	{
		if (0 == pTplOgre->mDropTable[i])
		{
			break;
		}
		int nItemID = CItemModule::Inst()->getFromDropTable(pTplOgre->mDropTable[i], nItemNum);
		CItemCreator tItemCreator;
		tItemCreator.mLineID = getLineID();
		tItemCreator.mMapID = getMapID();
		tItemCreator.mMapIndex = getMapIndex();
		tItemCreator.mNum = nItemNum;
		tItemCreator.mPos = getPos();
		tItemCreator.mTempID = nItemID;
		CMapModule::Inst()->createEntity(&tItemCreator);
	}

}

/// 设置毫秒级计时器(不能超过24天)
int CEntityOgre::setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes)
{
	if (mTimerList.getTimeListSize() >= MAX_NPC_TIMER_NUM)
	{
		return INVALID_OBJ_ID;
	}
	return mTimerList.setTimer(nOwerObjID, nModule, nMilliSec, pParam, nParamNum, nCallTimes);
}


/// 初始化
void CEntityFuncNPC::initEntity(CEntityCreator* pCreator)
{

}

void CEntityItem::init()
{
    mItemNum = 0;
}

/// 初始化
void CEntityItem::initEntity(CEntityCreator* pCreator)
{
	if (NULL == pCreator)
	{
		return;
	}

	if (emEntityType_Item == pCreator->getType())
	{
		return;
	}
	CItemCreator* pItemCreate = static_cast<CItemCreator*>(pCreator);
	mItemNum = pItemCreate->mNum;
}