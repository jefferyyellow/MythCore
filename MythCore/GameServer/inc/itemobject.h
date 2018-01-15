#ifndef __ITEMOBJECT_H__
#define __ITEMOBJECT_H__
#include "obj.h"
#include "itemtype.h"
class CItemObject;
class PBItemObject;
class CEntityPlayer;
#include "template.h"
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
protected:
	/// ����ID
	int			mItemID;
	/// ��������
	int			mItemNum;
};

// ͨ�õ��ߣ��ѵ��ĵ��߲������в���
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

// װ������
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
	/// �õ���������
	int	getProperty(int nPropertyType);
	/// ������������
	void setPropertyDirty(CEntityPlayer& rPlayer);
	// ˢ������
	void refreshProperty();
public:
	int		getLevel() const { return mLevel; }
	void	setLevel(int nValue) { mLevel = nValue; }

private:
	/// �ȼ�
	int			mLevel;
	int			mPropertyType[EQUIP_PROPERTY_NUM];			// ��������,ÿ��װ���������
	int			mPropertyValue[EQUIP_PROPERTY_NUM];			// ����ֵ
};
#endif