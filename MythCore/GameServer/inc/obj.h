#ifndef __OBJ_H__
#define __OBJ_H__

#include "logintype.h"

// 高8位为类型，低24位为ID
#define OBJ_ID_BITS 24
#define OBJ_TYPE(objID) ((objID >> OBJ_ID_BITS) & 0xFF)
#define OBJ_ID_START(ObjType)	(((ObjType) << OBJ_ID_BITS) + 1)
#define OBJ_ID_END(ObjType)	((((ObjType) + 1) << OBJ_ID_BITS) - 1)

enum  EmObjType
{
	emObjType_None				= 0,		// 未知
	emObjType_LoginPlayer		= 1,		// 登录玩家
	emObjType_Player			= 2,		// 玩家
};

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

class CLoginPlayer : public CObj
{
public:
	uint64		getKey(){return mKey;}
	void		setKey(uint64 nKey){mKey = nKey;}

	uint32		getRoleID() const { return mRoleID; }
	void		setRoleID(uint32 nValue) { mRoleID = nValue; }

	CExchangeHead&	GetExchangeHead(){return mExchangeHead;}

private:
	uint64			mKey;
	uint32			mRoleID;
	CExchangeHead	mExchangeHead;
};
#endif