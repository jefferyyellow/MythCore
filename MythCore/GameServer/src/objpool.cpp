#include "objpool.h"
#include "objpoolimp.h"
CObjPool::CObjPool()
{
	mPoolImp = new CObjPoolImp;
}

CObjPool::~CObjPool()
{
	if (NULL != mPoolImp)
	{
		delete mPoolImp;
		mPoolImp = NULL;
	}
}

int CObjPool::getShareMemorySize()
{
	int nSize = mPoolImp->mPlayerEntityPool.getMemorySize();
	nSize += mPoolImp->mItemCommonPool.getMemorySize();
	nSize += mPoolImp->mItemEquipPool.getMemorySize();
	return nSize;
}

void CObjPool::setShareMemory(byte* pShareMemory)
{
	mPoolImp->mPlayerEntityPool.init(pShareMemory);
	pShareMemory += mPoolImp->mPlayerEntityPool.getMemorySize();

	mPoolImp->mItemCommonPool.init(pShareMemory);
	pShareMemory += mPoolImp->mItemCommonPool.getMemorySize();

	mPoolImp->mItemEquipPool.init(pShareMemory);
}

CObj* CObjPool::allocObj(EmObjType eType)
{
	CObj* pObj = NULL;
	switch (eType)
	{
		case emObjType_LoginPlayer:
		{
			pObj = static_cast<CObj*>(mPoolImp->mLoginPlayerPool.allocObj());
			break;
		}
		case emObjType_Entity_Player:
		{
			pObj = static_cast<CObj*>(mPoolImp->mPlayerEntityPool.allocObj());
			break;
		}
		case emObjType_Entity_Ogre:
		{
			pObj = static_cast<CObj*>(mPoolImp->mOgreEntityPool.allocObj());
			break;
		}
		case emObjType_Entity_FuncNPC:
		{
			pObj = static_cast<CObj*>(mPoolImp->mFuncNPCEntityPool.allocObj());
			break;
		}
		case emObjType_Entity_Item:
		{
			pObj = static_cast<CObj*>(mPoolImp->mItemEntityPool.allocObj());
			break;
		}
		case emObjType_Item_Common:
		{
			pObj = static_cast<CObj*>(mPoolImp->mItemCommonPool.allocObj());
			break;
		}
		case emObjType_Item_Equip:
		{
			pObj = static_cast<CObj*>(mPoolImp->mItemEquipPool.allocObj());
			break;
		}
		case emObjType_Entity_Timer:
		{
			pObj = static_cast<CObj*>(mPoolImp->mEntityTimerPool.allocObj());
			break;
		}
		default:
			break;
	}

	return pObj;
}

CObj* CObjPool::getObj(int nObjID)
{
	CObj * pObj = NULL;
	switch (OBJ_TYPE(nObjID))
	{
		case emObjType_LoginPlayer:
		{
			pObj = static_cast<CObj*>(mPoolImp->mLoginPlayerPool.getObj(nObjID));
			break;
		}
		case emObjType_Entity_Player:
		{
			pObj = static_cast<CObj*>(mPoolImp->mPlayerEntityPool.getObj(nObjID));
			break;
		}
		case emObjType_Entity_Ogre:
		{
			pObj = static_cast<CObj*>(mPoolImp->mOgreEntityPool.getObj(nObjID));
			break;
		}
		case emObjType_Entity_FuncNPC:
		{
			pObj = static_cast<CObj*>(mPoolImp->mFuncNPCEntityPool.getObj(nObjID));
			break;
		}
		case emObjType_Entity_Item:
		{
			pObj = static_cast<CObj*>(mPoolImp->mItemEntityPool.getObj(nObjID));
			break;
		}
		case emObjType_Item_Common:
		{
			pObj = static_cast<CObj*>(mPoolImp->mItemCommonPool.getObj(nObjID));
			break;
		}
		case emObjType_Item_Equip:
		{
			pObj = static_cast<CObj*>(mPoolImp->mItemEquipPool.getObj(nObjID));
			break;
		}
		case emObjType_Entity_Timer:
		{
			pObj = static_cast<CObj*>(mPoolImp->mEntityTimerPool.getObj(nObjID));
			break;
		}
		default:
			break;
	}
	if (NULL == pObj)
	{
		return NULL;
	}
	if(INVALID_OBJ_ID == pObj->getObjID())
	{
		return NULL;
	}

	return pObj;
}

void CObjPool::free(int nObjID)
{
	switch (OBJ_TYPE(nObjID))
	{
		case emObjType_LoginPlayer:
		{
			mPoolImp->mLoginPlayerPool.freeByID(nObjID);
			break;
		}
		case emObjType_Entity_Player:
		{
			mPoolImp->mPlayerEntityPool.freeByID(nObjID);
			break;
		}
		case emObjType_Entity_Ogre:
		{
			mPoolImp->mOgreEntityPool.freeByID(nObjID);
			break;
		}
		case emObjType_Entity_FuncNPC:
		{
			mPoolImp->mFuncNPCEntityPool.freeByID(nObjID);
			break;
		}
		case emObjType_Entity_Item:
		{
			mPoolImp->mItemEntityPool.freeByID(nObjID);
			break;
		}
		case emObjType_Item_Common:
		{
			mPoolImp->mItemCommonPool.freeByID(nObjID);
			break;
		}
		case emObjType_Item_Equip:
		{
			mPoolImp->mItemEquipPool.freeByID(nObjID);
			break;
		}
		case emObjType_Entity_Timer:
		{
			mPoolImp->mEntityTimerPool.freeByID(nObjID);
			break;
		}
		default:
			break;
	}
}

void CObjPool::logObjNum()
{
	char acBuffer[STR_LENGTH_1024] = {0};
	int nLen = 0;
	int nMaxLen= sizeof(acBuffer) - 1;
	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "\n%20s\t%6s\t%6s\n", "name", "usage", "total");

	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "%20s\t%6d\t%6d\n",
		"mLoginPlayerPool",
		mPoolImp->mLoginPlayerPool.getObjNum(),
		mPoolImp->mLoginPlayerPool.getMaxNum());

	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "%20s\t%6d\t%6d\n",
		"mPlayerEntityPool",
		mPoolImp->mPlayerEntityPool.getObjNum(),
		mPoolImp->mPlayerEntityPool.getMaxNum());

	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "%20s\t%6d\t%6d\n",
		"mOgreEntityPool",
		mPoolImp->mOgreEntityPool.getObjNum(),
		mPoolImp->mOgreEntityPool.getMaxNum());

	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "%20s\t%6d\t%6d\n",
		"mFuncNPCEntityPool",
		mPoolImp->mFuncNPCEntityPool.getObjNum(),
		mPoolImp->mFuncNPCEntityPool.getMaxNum());

	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "%20s\t%6d\t%6d\n",
		"mItemEntityPool",
		mPoolImp->mItemEntityPool.getObjNum(),
		mPoolImp->mItemEntityPool.getMaxNum());

	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "%20s\t%6d\t%6d\n",
		"mItemCommonPool",
		mPoolImp->mItemCommonPool.getObjNum(),
		mPoolImp->mItemCommonPool.getMaxNum());

	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "%20s\t%6d\t%6d\n",
		"mItemEquipPool",
		mPoolImp->mItemEquipPool.getObjNum(),
		mPoolImp->mItemEquipPool.getMaxNum());

	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "%20s\t%6d\t%6d\n",
		"mEntityTimerPool",
		mPoolImp->mEntityTimerPool.getObjNum(),
		mPoolImp->mEntityTimerPool.getMaxNum());

	nLen += snprintf(acBuffer + nLen, nMaxLen - nLen, "************************************************");

	LOG_INFO(acBuffer);
}

int CObjPool::getPlayerMinID()
{
	return mPoolImp->mPlayerEntityPool.getMinID();
}

int CObjPool::getPlayerMaxID()
{
	return mPoolImp->mPlayerEntityPool.getMaxID();
}