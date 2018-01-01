#ifndef __ENTITYCREATOR_H__
#define __ENTITYCREATOR_H__

/// ����������
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
	/// ʵ������
	EmEntityType			mType;
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
	COgreCreator()
	{
		mType = emEntityType_Item;
		mNum = 0;
		memset(mOwnerID, 0, sizeof(mOwnerID));
	}
	~COgreCreator(){}
public:
	/// ������Ŀ
	int						mNum;
	/// ����������
	int						mOwnerID[TEAM_MEM_NUM];
};

#endif