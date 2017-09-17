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
	typedef CObjManager<CLoginPlayer, 64, 32, 32, OBJ_ID_RANGE(emObjType_LoginPlayer)> LoginPlayerPool;
	typedef CObjManager<CEntityPlayer, 2000, 30, 30, OBJ_ID_RANGE(emObjType_Entity_Player)> PlayerEntityPool;
	typedef CObjManager<CEntityOgre, 100000, 1000, 1000, OBJ_ID_RANGE(emObjType_Entity_Ogre)> OgreEntityPool;
	typedef CObjManager<CEntityFuncNPC, 100000, 1000, 1000, OBJ_ID_RANGE(emObjType_Entity_FuncNPC)> FuncNPCEntityPool;
	typedef CObjManager<CEntityItem, 100000, 1000, 1000, OBJ_ID_RANGE(emObjType_Entity_Item)> ItemEntityPool;

public:
	LoginPlayerPool			mLoginPlayerPool;
	PlayerEntityPool		mPlayerEntityPool;
	OgreEntityPool			mOgreEntityPool;
	FuncNPCEntityPool		mFuncNPCEntityPool;
	ItemEntityPool			mItemEntityPool;
};
#endif