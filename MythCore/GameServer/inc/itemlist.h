#ifndef __ITEMLIST_H__
#define __ITEMLIST_H__
#include "objpool.h"
template<uint32 Capacity>
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
		for (int i = 0; i < Capacity; ++i)
		{
			mItemObjID[i] = INVALID_OBJ_ID;
		}
	}

	void		clear()
	{
		for (int i = 0; i < Capacity; ++ i)
		{
			if (INVALID_OBJ_ID != mItemObjID[i])
			{
				//CObjPool::Inst()->free(mItemObjID[i]);
			}
		}
	}

	void		setItem(int nIndex, uint32 nObjID, uint32 nItemID)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return;
		}

		mItemObjID[nIndex] = nObjID;
		mItemID[nItemID] = nItemID;
	}

public:
	void		setItemObjID(int nIndex, uint32 nObjID)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return;
		}

		mItemObjID[nIndex] = nObjID;
	}

	uint32		operator[](int nIndex)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return INVALID_OBJ_ID;
		}
		return mItemObjID[nIndex];
	}

	uint32		getItemObjID(int nIndex)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return INVALID_OBJ_ID;
		}
		return mItemObjID[nIndex];
	}

	void		getItemID(int nIndex)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return;
		}

		return mItemID[nIndex];
	}
	uint32		getCapacity(){return Capacity;}

protected:
	/// Obj ID列表
	uint32		mItemObjID[Capacity];
	/// 道具模板ID
	uint32		mItemID[Capacity];
};
#endif