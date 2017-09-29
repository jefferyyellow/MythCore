#ifndef __ITEMOBJECT_H__
#define __ITEMOBJECT_H__
#include "obj.h"

class CItemObject;
class CItemFactory
{
public:
	static CItemObject* createItem(int nItemID);
	static void destroyItem(int nObjID);
};

class CItemObject : public CObj
{
public:
	CItemObject()
	{
		mItemID = 0;
		mItemNum = 0;
	}

public:
	uint32 GetItemID() const { return mItemID; }
	void SetItemID(uint32 nValue) { mItemID = nValue; }

	uint32 GetItemNum() const { return mItemNum; }
	void SetItemNum(uint32 nValue) { mItemNum = nValue; }

private:
	/// ����ID
	uint32			mItemID;
	/// ��������
	uint32			mItemNum;
};

// ͨ�õ���
class CItemCommon : public CItemObject
{
public:
	CItemCommon()
	{
		mParam1 = 0;
		mParam2 = 0;
	}
	~CItemCommon(){}

public:
	uint32	getParam1() const { return mParam1; }
	void	getParam1(uint32 nValue) { mParam1 = nValue; }

	uint32	getParam2() const { return mParam2; }
	void	setParam2(uint32 nValue) { mParam2 = nValue; }

private:
	/// ����1����ͬ�ĵ��߲�ͬ
	uint32			mParam1;
	/// ����2����ͬ�ĵ��߲�ͬ
	uint32			mParam2;
};

// װ������
class CItemEquip : public CItemObject
{
public:
	CItemEquip()
	{
		mLevel = 0;
	}
	~CItemEquip()
	{}
public:
	uint32	getLevel() const { return mLevel; }
	void	setLevel(uint32 nValue) { mLevel = nValue; }
	
private:
	/// �ȼ�
	uint32			mLevel;
};
#endif