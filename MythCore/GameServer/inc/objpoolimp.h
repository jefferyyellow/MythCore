#ifndef __OBJPOOLIMP_H__
#define __OBJPOOLIMP_H__
#include "servercommon.h"
#include "obj.h"
#include "objmanager.h"
#include "entityplayer.h"
#include "itemobject.h"
#include "loginplayer.h"
#include "entitytimer.h"
#include "instance.h"
#define OBJ_ID_RANGE(ObjType) OBJ_ID_START(ObjType),OBJ_ID_END(ObjType)


#define CAPACITY_LOGIN_PLAYER		256
#define	CAPACITY_PLAYER				2000
#define CAPACITY_ENTITY_OGRE		100000
#define CAPACITY_ENTITY_FUNC_NPC	300
#define CAPACITY_ENTITY_ITEM		CAPACITY_PLAYER * 10
#define CAPACITY_ITEM_COMMON		CAPACITY_PLAYER * 200
#define CAPACITY_ITEM_EQUIP			CAPACITY_PLAYER * 200
#define CAPACITY_ENTITY_TIMER		200000	// 20��ļ�ʱ��
#define CAPACITY_INSTANCE			CAPACITY_PLAYER

class CShareObjPoolImp
{
public:
	/// �������ص�һ��Ҫ�������ʹ�ù����ڴ�ķ�ʽ,��������ģ�ע���CObjPool::getShareMemorySize�ͳ�ʼ��
	typedef CShareObjManager<CEntityPlayer, CAPACITY_PLAYER, OBJ_ID_RANGE(emObjType_Entity_Player)> PlayerEntityPool;
	typedef CShareObjManager<CItemCommon, CAPACITY_ITEM_COMMON, OBJ_ID_RANGE(emObjType_Item_Common)> ItemCommonPool;
	typedef CShareObjManager<CItemEquip, CAPACITY_ITEM_EQUIP, OBJ_ID_RANGE(emObjType_Item_Equip)> ItemEquipPool;

public:
	PlayerEntityPool		mPlayerEntityPool;			// ���ʵ���
	ItemCommonPool			mItemCommonPool;			// ͨ�õ���ʵ���
	ItemEquipPool			mItemEquipPool;				// װ������ʵ���
};

class CObjPoolImp
{
public:
	typedef CObjManager<CLoginPlayer, CAPACITY_LOGIN_PLAYER, 32, 32, OBJ_ID_RANGE(emObjType_LoginPlayer)> LoginPlayerPool;
	typedef CObjManager<CEntityOgre, CAPACITY_ENTITY_OGRE, 1000, 100, OBJ_ID_RANGE(emObjType_Entity_Ogre)> OgreEntityPool;
	typedef CObjManager<CEntityFuncNPC, CAPACITY_ENTITY_FUNC_NPC, 30, 30, OBJ_ID_RANGE(emObjType_Entity_FuncNPC)> FuncNPCEntityPool;
	typedef CObjManager<CEntityItem, CAPACITY_ENTITY_ITEM, 100, 100, OBJ_ID_RANGE(emObjType_Entity_Item)> ItemEntityPool;
	typedef CObjManager<CEntityTimer, CAPACITY_ENTITY_TIMER, 1000, 1000, OBJ_ID_RANGE(emObjType_Entity_Timer)> EntityTimerPool;
	typedef CObjManager<CInstance, CAPACITY_INSTANCE, 100, 100, OBJ_ID_RANGE(emObjType_Instance)> InstancePool;

public:
	LoginPlayerPool			mLoginPlayerPool;			// ��¼��ҳ�
	OgreEntityPool			mOgreEntityPool;			// ����ʵ���
	FuncNPCEntityPool		mFuncNPCEntityPool;			// ����NPCʵ���
	ItemEntityPool			mItemEntityPool;			// ����ʵ���
	EntityTimerPool			mEntityTimerPool;			// ʵ���ʱ����
	InstancePool			mInstancePool;				// ������
};
#endif