#include "objpool.h"
CObj* CObjPool::allocObj(EmObjType eType)
{
	CObj* pObj = NULL;
	switch (eType)
	{
		case emObjType_LoginPlayer:
		{
			pObj = reinterpret_cast<CObj*>(mLoginPlayerPool.allocObj());
			break;
		}
		case emObjType_Player:
		{
			pObj = reinterpret_cast<CObj*>(mPlayerPool.allocObj());
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
			pObj = reinterpret_cast<CObj*>(mLoginPlayerPool.getObj(nObjID));
			break;
		}
		case emObjType_Player:
		{
			pObj = reinterpret_cast<CObj*>(mPlayerPool.getObj(nObjID));
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
			mLoginPlayerPool.freeByID(nObjID);
			break;
		}
		case emObjType_Player:
		{
			mPlayerPool.freeByID(nObjID);
			break;
		}
		default:
			break;
	}
}