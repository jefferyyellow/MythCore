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
	int GetItemID() const { return mItemID; }
	void SetItemID(int nValue) { mItemID = nValue; }

	int GetItemNum() const { return mItemNum; }
	void SetItemNum(int nValue) { mItemNum = nValue; }

private:
	/// ����ID
	int			mItemID;
	/// ��������
	int			mItemNum;
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
	int		getParam1() const { return mParam1; }
	void	getParam1(int nValue) { mParam1 = nValue; }

	int		getParam2() const { return mParam2; }
	void	setParam2(int nValue) { mParam2 = nValue; }

private:
	/// ����1����ͬ�ĵ��߲�ͬ
	int			mParam1;
	/// ����2����ͬ�ĵ��߲�ͬ
	int			mParam2;
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
	int	getLevel() const { return mLevel; }
	void	setLevel(int nValue) { mLevel = nValue; }
	
private:
	/// �ȼ�
	int			mLevel;
};
#endif