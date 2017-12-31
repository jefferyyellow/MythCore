#ifndef __ITEMOBJECT_H__
#define __ITEMOBJECT_H__
#include "obj.h"
#include "itemtype.h"
class CItemObject;
class PBItemObject;
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

	virtual void setFromPB(PBItemObject* pItemObject) = 0;
	virtual void createToPB(PBItemObject* pItemObject) = 0;
private:
	/// 道具ID
	int			mItemID;
	/// 道具数量
	int			mItemNum;
};

// 通用道具，堆叠的道具不可能有参数
class CItemCommon : public CItemObject
{
public:
	CItemCommon()
	{

	}
	~CItemCommon(){}

public:
	virtual void setFromPB(PBItemObject* pItemObject);
	virtual void createToPB(PBItemObject* pItemObject);

};

// 装备道具
class CItemEquip : public CItemObject
{
public:
	CItemEquip()
	{
		mLevel = 0;
	}
	virtual ~CItemEquip()
	{}

	virtual void setFromPB(PBItemObject* pItemObject);
	virtual void createToPB(PBItemObject* pItemObject);

public:
	int	getProperty(int nPropertyType);

public:
	int		getLevel() const { return mLevel; }
	void	setLevel(int nValue) { mLevel = nValue; }

private:
	/// 等级
	int			mLevel;
	int			mPropertyType[EQUIP_PROPERTY_NUM];			// 属性类型,每种装备最多六种
	int			mPropertyValue[EQUIP_PROPERTY_NUM];				// 属性值

};
#endif