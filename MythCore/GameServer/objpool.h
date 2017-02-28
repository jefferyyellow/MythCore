#ifndef __OBJPOOL_H__
#define __OBJPOOL_H__
#include "commontype.h"
#include "obj.h"
#include "objmanager.h"
#include "singleton.h"
#include "entity.h"
#define  OBJ_ID_RANGE(ObjType) OBJ_ID_START(ObjType),OBJ_ID_END(ObjType)

class CObjPool : public CSingleton <CObjPool>
{
	friend class CSingleton<CObjPool>;
private:
	CObjPool()
	{
	}
	~CObjPool(){}
	
public:
	typedef CObjManager<CLoginPlayer, 1024, 32, 32, OBJ_ID_RANGE(emObjType_LoginPlayer)> LoginPlayerPool;
	typedef CObjManager<CEntityPlayer, 3000, 30, 30, OBJ_ID_RANGE(emObjType_Player)> PlayerPool;

public:
	CObj*	allocObj(EmObjType eType);
	CObj*	getObj(uint32 nObjID);
	void	free(uint32 nObjID);

private:
	LoginPlayerPool		mLoginPlayerPool;
	PlayerPool			mPlayerPool;
};
#endif