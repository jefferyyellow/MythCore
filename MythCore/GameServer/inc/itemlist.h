#ifndef __ITEMLIST_H__
#define __ITEMLIST_H__
#include "objpool.h"
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

	void		getItemID(int nIndex)
	{
		if (nIndex < 0 || nIndex >= Capacity)
		{
			return;
		}

		return mItemID[nIndex];
	}
	int		getCapacity(){return Capacity;}

protected:
	/// Obj ID列表
	int		mItemObjID[Capacity];
	/// 道具模板ID
	int		mItemID[Capacity];
};
#endif