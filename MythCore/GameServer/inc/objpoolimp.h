#ifndef __OBJPOOLIMP_H__
#define __OBJPOOLIMP_H__
#include "commontype.h"
#include "obj.h"
#include "objmanager.h"
#include "entityplayer.h"
#define  OBJ_ID_RANGE(ObjType) OBJ_ID_START(ObjType),OBJ_ID_END(ObjType)
class CObjPoolImp
{
public:
	typedef CObjManager<CLoginPlayer, 1024, 32, 32, OBJ_ID_RANGE(emObjType_LoginPlayer)> LoginPlayerPool;
	typedef CObjManager<CEntityPlayer, 3000, 30, 30, OBJ_ID_RANGE(emObjType_Player)> PlayerPool;

public:
	LoginPlayerPool		mLoginPlayerPool;
	PlayerPool			mPlayerPool;
};
#endif