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
		pObj = static_cast<CObj*>(mPoolImp->mLoginPlayerPool.allocObj());
		break;
	}
	case emObjType_Entity_Player:
	{
		pObj = static_cast<CObj*>(mPoolImp->mPlayerEntityPool.allocObj());
		break;
	}
	case emObjType_Room:
	{
		pObj = static_cast<CObj*>(mPoolImp->mRoomPool.allocObj());
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
	case emObjType_Room:
	{
		pObj = static_cast<CObj*>(mPoolImp->mRoomPool.getObj(nObjID));
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
	case emObjType_Room:
	{
		mPoolImp->mRoomPool.freeByID(nObjID);
		break;
	}
	default:
		break;
	}
}