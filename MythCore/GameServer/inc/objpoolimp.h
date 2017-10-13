#ifndef __OBJPOOLIMP_H__
#define __OBJPOOLIMP_H__
#include "commontype.h"
#include "obj.h"
#include "objmanager.h"
#include "entityplayer.h"
#include "itemobject.h"
#include "loginplayer.h"
#define OBJ_ID_RANGE(ObjType) OBJ_ID_START(ObjType),OBJ_ID_END(ObjType)
#define CAPACITY_LOGIN_PLAYER		256
#define	CAPACITY_PLAYER				2000
#define CAPACITY_ENTITY_OGRE		100000
#define CAPACITY_ENTITY_FUNC_NPC	300
#define CAPACITY_ENTITY_ITEM		CAPACITY_PLAYER * 10
#define CAPACITY_ITEM_COMMON		CAPACITY_PLAYER * 200
#define CAPACITY_ITEM_EQUIP			CAPACITY_PLAYER * 200
class CObjPoolImp
{
public:
	typedef CObjManager<CLoginPlayer, CAPACITY_LOGIN_PLAYER, 32, 32, OBJ_ID_RANGE(emObjType_LoginPlayer)> LoginPlayerPool;
	typedef CObjManager<CEntityPlayer, CAPACITY_PLAYER, 20, 20, OBJ_ID_RANGE(emObjType_Entity_Player)> PlayerEntityPool;
	typedef CObjManager<CEntityOgre, CAPACITY_ENTITY_OGRE, 1000, 100, OBJ_ID_RANGE(emObjType_Entity_Ogre)> OgreEntityPool;
	typedef CObjManager<CEntityFuncNPC, CAPACITY_ENTITY_FUNC_NPC, 30, 30, OBJ_ID_RANGE(emObjType_Entity_FuncNPC)> FuncNPCEntityPool;
	typedef CObjManager<CEntityItem, CAPACITY_ENTITY_ITEM, 100, 100, OBJ_ID_RANGE(emObjType_Entity_Item)> ItemEntityPool;
	typedef CObjManager<CItemCommon, CAPACITY_ITEM_COMMON, 1000, 1000, OBJ_ID_RANGE(emObjType_Item_Common)> ItemCommonPool;
	typedef CObjManager<CItemEquip, CAPACITY_ITEM_EQUIP, 1000, 1000, OBJ_ID_RANGE(emObjType_Item_Equip)> ItemEquipPool;

public:
	LoginPlayerPool			mLoginPlayerPool;			// ��¼��ҳ�
	PlayerEntityPool		mPlayerEntityPool;			// ���ʵ���
	OgreEntityPool			mOgreEntityPool;			// ����ʵ���
	FuncNPCEntityPool		mFuncNPCEntityPool;			// ����NPCʵ���
	ItemEntityPool			mItemEntityPool;			// ����ʵ���
	ItemCommonPool			mItemCommonPool;			// ͨ�õ���ʵ���
	ItemEquipPool			mItemEquipPool;				// װ������ʵ���
};
#endif