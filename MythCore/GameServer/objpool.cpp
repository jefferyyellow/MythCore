#include "objpool.h"
CObj* CObjPool::allocObj(EmObjType eType)
{
	CObj* pObj = NULL;
	switch (eType)
	{
		case emObjType_LoginPlayer:
			pObj = reinterpret_cast<CObj*>(mLoginPlayerPool.allocObj());
			break;
		default:
			break;
	}

	return pObj;
}

CObj* CObjPool::getObj(uint32 nObjID)
{
	switch (OBJ_TYPE(nObjID))
	{
		case emObjType_LoginPlayer:
			mLoginPlayerPool.getObj(nObjID);
			break;
		default:
			break;
	}

	return NULL;
}

void CObjPool::free(uint32 nObjID)
{
	switch (OBJ_TYPE(nObjID))
	{
		case emObjType_LoginPlayer:
			mLoginPlayerPool.freeByID(nObjID);
			break;
		default:
			break;
	}
}