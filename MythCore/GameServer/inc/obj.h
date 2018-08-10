#ifndef __OBJ_H__
#define __OBJ_H__

#include "logintype.h"

// ��8λΪ���ͣ���24λΪID
#define OBJ_ID_BITS 24
#define OBJ_TYPE(objID) ((objID >> OBJ_ID_BITS) & 0xFF)
#define OBJ_ID_START(ObjType)	(((ObjType) << OBJ_ID_BITS) + 1)
#define OBJ_ID_END(ObjType)	((((ObjType) + 1) << OBJ_ID_BITS) - 1)

class CObj
{
public:
	CObj()
	{
		mObjID = 0;
	}
	~CObj(){}

public:
	int			getObjID(){ return mObjID; }
	void		setObjID(int nObjID){ mObjID = nObjID; }

private:
	int			mObjID;
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
	emObjType_Entity_Timer			= 8,	// ʵ���ʱ��
};


#endif