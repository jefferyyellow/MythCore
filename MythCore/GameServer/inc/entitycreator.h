#ifndef __ENTITYCREATOR_H__
#define __ENTITYCREATOR_H__
#include "entitytype.h"
#include "servercommon.h"
/// ����������
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
	/// ʵ������
	EmEntityType			mType;

public:
	/// ģ��ID
	int						mTempID;
	/// ��ID
	unsigned short			mLineID;
	/// ��ͼID
	unsigned short			mMapID;
	/// ��ͼ����
	int						mMapIndex;
	/// ��ͼ�е�λ��
	CMythPoint				mPos;
};

// ���ߴ�����
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
	/// ������Ŀ
	int						mNum;
	/// ����������
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