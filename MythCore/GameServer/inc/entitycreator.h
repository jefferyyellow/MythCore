#ifndef __ENTITYCREATOR_H__
#define __ENTITYCREATOR_H__
#include "entitytype.h"
#include "servercommon.h"
/// 创建器基类
class CEntityCreator
{
public:
	CEntityCreator()
	{
		mType = emEntityType_None;
		init();
	};
	~CEntityCreator(){};

	void init()
	{
        mTempID = 0;
        mLineID = 0;
        mMapID = 0;
        mMapIndex = 0;
	}
public:
	EmEntityType getType()const{return mType;}
	void setType(EmEntityType val){	mType = val;}

protected:
	/// 实体类型
	EmEntityType			mType;

public:
	/// 模板ID
	int						mTempID;
	/// 线ID
	unsigned short			mLineID;
	/// 地图ID
	unsigned short			mMapID;
	/// 地图索引
	int						mMapIndex;
	/// 地图中的位置
	CMythPoint				mPos;
};

// 道具创建器
class CItemCreator : public CEntityCreator
{
public:
	CItemCreator()
	{
		mType = emEntityType_Item;
		init();
	}
	~CItemCreator(){}
	void init()
	{
        mNum = 0;
        memset(mOwnerID, 0, sizeof(mOwnerID));
	}
public:
	/// 道具数目
	int						mNum;
	/// 道具所有者
	int						mOwnerID[TEAM_MEM_NUM];
};

class CNpcCreator : public CEntityCreator
{
public:
	CNpcCreator()
	{
		mType = emEntityType_FuncNPC;
	}
	~CNpcCreator(){}
};

class COgreCreator : public CEntityCreator
{
public:
	COgreCreator()
	{
		mType = emEntityType_Item;
	}
	~COgreCreator(){}
};
#endif