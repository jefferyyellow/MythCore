#include "objpool.h"
#include "objpoolimp.h"
CObjPool::CObjPool()
{
	mPoolImp = new CObjPoolImp;
}

CObj* CObjPool::allocObj(EmObjType eType)
{
	CObj* pObj = NULL;
	switch (eType)
	{
	case emObjType_LoginPlayer:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mLoginPlayerPool.allocObj());
		break;
	}
	case emObjType_Entity_Player:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mPlayerEntityPool.allocObj());
		break;
	}
	case emObjType_Entity_Ogre:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mOgreEntityPool.allocObj());
		break;
	}
	case emObjType_Entity_FuncNPC:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mFuncNPCEntityPool.allocObj());
		break;
	}
	case emObjType_Entity_Item:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mItemEntityPool.allocObj());
		break;
	}
	case emObjType_Item_Common:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mItemCommonPool.allocObj());
		break;
	}
	case emObjType_Item_Equip:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mItemEquipPool.allocObj());
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
		pObj = reinterpret_cast<CObj*>(mPoolImp->mLoginPlayerPool.getObj(nObjID));
		break;
	}
	case emObjType_Entity_Player:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mPlayerEntityPool.getObj(nObjID));
		break;
	}
	case emObjType_Entity_Ogre:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mOgreEntityPool.getObj(nObjID));
		break;
	}
	case emObjType_Entity_FuncNPC:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mFuncNPCEntityPool.getObj(nObjID));
		break;
	}
	case emObjType_Entity_Item:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mItemEntityPool.getObj(nObjID));
		break;
	}
	case emObjType_Item_Common:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mItemCommonPool.getObj(nObjID));
		break;
	}
	case emObjType_Item_Equip:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mItemEquipPool.getObj(nObjID));
		break;
	}
	default:
		break;
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
	default:
		break;
	}
}