#include "objpool.h"
#include "objpoolimp.h"
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
	case emObjType_Player:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mPlayerPool.allocObj());
		break;
	}
	default:
		break;
	}

	return pObj;
}

CObj* CObjPool::getObj(uint32 nObjID)
{
	CObj * pObj = NULL;
	switch (OBJ_TYPE(nObjID))
	{
	case emObjType_LoginPlayer:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mLoginPlayerPool.getObj(nObjID));
		break;
	}
	case emObjType_Player:
	{
		pObj = reinterpret_cast<CObj*>(mPoolImp->mPlayerPool.getObj(nObjID));
		break;
	}
	default:
		break;
	}

	return pObj;
}

void CObjPool::free(uint32 nObjID)
{
	switch (OBJ_TYPE(nObjID))
	{
	case emObjType_LoginPlayer:
	{
		mPoolImp->mLoginPlayerPool.freeByID(nObjID);
		break;
	}
	case emObjType_Player:
	{
		mPoolImp->mPlayerPool.freeByID(nObjID);
		break;
	}
	default:
		break;
	}
}