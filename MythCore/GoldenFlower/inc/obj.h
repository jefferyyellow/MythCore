#ifndef __OBJ_H__
#define __OBJ_H__

#include "logintype.h"

// 高8位为类型，低24位为ID
#define OBJ_ID_BITS 24
#define OBJ_TYPE(objID) ((objID >> OBJ_ID_BITS) & 0xFF)
#define OBJ_ID_START(ObjType)	(((ObjType) << OBJ_ID_BITS) + 1)
#define OBJ_ID_END(ObjType)	((((ObjType) + 1) << OBJ_ID_BITS) - 1)

#define INVALID_OBJ_ID 0		// 非法的OBJ ID
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
	emObjType_None					= 0,	// 未知
	emObjType_LoginPlayer			= 1,	// 登录玩家
	emObjType_Entity_Player			= 2,	// 玩家
	emObjType_Room					= 3,	// 房间
};


#endif