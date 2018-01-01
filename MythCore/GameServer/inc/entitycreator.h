#ifndef __ENTITYCREATOR_H__
#define __ENTITYCREATOR_H__

/// 创建器基类
class CEntityCreator
{
public:
	CEntityCreator()
	{
		mType = emEntityType_None;
		mTempID = 0;
		mLineID = 0;
		mMapID = 0;
		mMapIndex = 0;
	};
	~CEntityCreator(){};

public:
	/// 实体类型
	EmEntityType			mType;
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
	COgreCreator()
	{
		mType = emEntityType_Item;
		mNum = 0;
		memset(mOwnerID, 0, sizeof(mOwnerID));
	}
	~COgreCreator(){}
public:
	/// 道具数目
	int						mNum;
	/// 道具所有者
	int						mOwnerID[TEAM_MEM_NUM];
};

#endif