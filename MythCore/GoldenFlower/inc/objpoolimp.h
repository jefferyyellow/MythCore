#ifndef __OBJPOOLIMP_H__
#define __OBJPOOLIMP_H__
#include "servercommon.h"
#include "obj.h"
#include "objmanager.h"
#include "entityplayer.h"
#include "loginplayer.h"
#include "room.h"
#define OBJ_ID_RANGE(ObjType) OBJ_ID_START(ObjType),OBJ_ID_END(ObjType)
#define CAPACITY_LOGIN_PLAYER		256
#define	CAPACITY_PLAYER				2000
#define CAPACITY_ROOM				2000

class CObjPoolImp
{
public:
	typedef CObjManager<CLoginPlayer, CAPACITY_LOGIN_PLAYER, 32, 32, OBJ_ID_RANGE(emObjType_LoginPlayer)> LoginPlayerPool;
	typedef CObjManager<CEntityPlayer, CAPACITY_PLAYER, 20, 20, OBJ_ID_RANGE(emObjType_Entity_Player)> PlayerEntityPool;
	typedef CObjManager<CRoom,CAPACITY_ROOM, 100, 100, OBJ_ID_RANGE(emObjType_Room)> RoomPool;
public:
	LoginPlayerPool			mLoginPlayerPool;			// 登录玩家池
	PlayerEntityPool		mPlayerEntityPool;			// 玩家实体池
	RoomPool				mRoomPool;
};
#endif