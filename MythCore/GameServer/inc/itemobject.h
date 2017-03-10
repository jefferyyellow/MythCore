#ifndef __ITEMOBJECT_H__
#define __ITEMOBJECT_H__
#include "obj.h"
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
	uint32			mItemID;
	uint32			mItemNum;
};
#endif