#ifndef __OBJ_H__
#define __OBJ_H__

#include "logintype.h"

// ��8λΪ���ͣ���24λΪID
#define OBJ_ID_BITS 24
#define OBJ_TYPE(objID) ((objID >> OBJ_ID_BITS) & 0xFF)
#define OBJ_ID_START(ObjType)	(((ObjType) << OBJ_ID_BITS) + 1)
#define OBJ_ID_END(ObjType)	((((ObjType) + 1) << OBJ_ID_BITS) - 1)

#define INVALID_OBJ_ID 0		// �Ƿ���OBJ ID
class CObj
{
public:
	CObj()
	{
		mObjID = 0;
	}
	~CObj(){}

public:
	uint32		getObjID(){ return mObjID; }
	void		setObjID(uint32 nObjID){ mObjID = nObjID; }

private:
	uint32		mObjID;
};

enum  EmObjType
{
	emObjType_None					= 0,	// δ֪
	emObjType_LoginPlayer			= 1,	// ��¼���
	emObjType_Entity_Player			= 2,	// ���
	emObjType_Entity_Ogre			= 3,	// ����
	emObjType_Entity_FuncNPC		= 4,	// ����NPC
	emObjType_Entity_Item			= 5,	// �������
	emObjType_Item_Common			= 6,	// ��ͨ����
	emObjType_Item_Equip			= 7,	// װ������
};


#endif