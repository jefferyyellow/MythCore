#ifndef __ITEMLIST_H__
#define __ITEMLIST_H__
#include "objpool.h"
#include "itemobject.h"
#include "common.hxx.pb.h"
class PBItemObject;
class PBItemList;
template<int Capacity>
class CItemList
{
public:
	CItemList()
	{
		init();
	}
	~CItemList()
	{
		clear();
	}

public:
	void		init()
	{
        memset(mItemObjID, 0, sizeof(mItemObjID));
        memset(mItemID, 0, sizeof(mItemID));
	}

	void		clear()
	{
		for (int i = 0; i < Capacity; ++ i)
		{
			if (INVALID_OBJ_ID != mItemObjID[i])
			{
				CObjPool::Inst()->free(mItemObjID[i]);
				mItemID[i] = 0;
				mItemObjID[i] = INVALID_OBJ_ID;
			}
		}
	}

	void		setItem(int nIndex, int nObjID, int nItemID)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return;
		}

		mItemObjID[nIndex] = nObjID;
		mItemID[nItemID] = nItemID;
	}

public:
	void		setItemObjID(int nIndex, int nObjID)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return;
		}

		mItemObjID[nIndex] = nObjID;
	}

	int		operator[](int nIndex)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return INVALID_OBJ_ID;
		}
		return mItemObjID[nIndex];
	}

	int		getItemObjID(int nIndex)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return INVALID_OBJ_ID;
		}
		return mItemObjID[nIndex];
	}

	int		getItemID(int nIndex)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return 0;
		}

		return mItemID[nIndex];
	}
	int		getCapacity(){return Capacity;}

	void setFromPB(PBItemList* pbItemList)
	{
		if (NULL == pbItemList)
		{
			return;
		}
		for (int i = 0; i < pbItemList->itemobject_size() && i < Capacity; ++i)
		{
			PBItemObject* pbObject = pbItemList->mutable_itemobject(i);
			if (NULL == pbObject)
			{
				continue;
			}
			int nIndex = pbObject->index();
			if (nIndex < 0 || nIndex >= Capacity)
			{
				continue;
			}
			int nItemID = pbObject->itemid();
			CItemObject* pItemObject = CItemFactory::createItem(nItemID);
			if (NULL == pItemObject)
			{
				continue;
			}

			pItemObject->setFromPB(pbObject);
			mItemObjID[nIndex] = pItemObject->getObjID();
			mItemID[nIndex] = nItemID;
		}

	}

	void createToPB(PBItemList* pbItemList, int nSize = Capacity)
	{
		if (NULL == pbItemList)
		{
			return;
		}
		for (int i = 0; i < nSize; ++ i)
		{
			if (INVALID_OBJ_ID == mItemObjID[i])
			{
				continue;
			}

			CItemObject* pItemObject = static_cast<CItemObject*>(CObjPool::Inst()->getObj(mItemObjID[i]));
			if (NULL == pItemObject)
			{
				continue;
			}
			PBItemObject* pbObject = pbItemList->add_itemobject();
			if (NULL == pbObject)
			{
				continue;
			}
			pbObject->set_index(i);
			pItemObject->createToPB(pbObject);
		}

	}

	CItemObject* getItem(unsigned int nIndex)
	{
		if (nIndex >= (unsigned int)Capacity)
		{
			return NULL;
		}
		if (INVALID_OBJ_ID == mItemObjID[nIndex])
		{
			return NULL;
		}

		return static_cast<CItemObject*>(CObjPool::Inst()->getObj(mItemObjID[nIndex]));
	}
protected:
	/// Obj ID列表
	int		mItemObjID[Capacity];
	/// 道具模板ID
	int		mItemID[Capacity];
};
#endif