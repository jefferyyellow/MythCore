#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__
#define MAX_INSERT_TYPE_NUM 16
#include "itemlist.h"
#define  MAX_CONTAINER_ITEM_NUM 100
class CItemContainer : public CItemList < MAX_CONTAINER_ITEM_NUM >
{
public:
	CItemContainer()
	{
		mSize = 0;
	}
	~CItemContainer()
	{

	}

public:
	/// 是否有足够的空间放下这一堆道具
	bool	checkSpace(int* pItemID, int* pNumber, int nSize);
	// 是否有足够的空间放指定的道具
	bool	checkSpace(int nItemID, int nNumber);
	// 插入道具
	int		insertItem(int nItemID, int nItemNum, int *pOutIndex, int *pOutNumber, int &rOutLen);
	/// 背包里是否有足够的道具
	bool	checkEnough(int nItemID, int nItemNum);
	/// 删除道具
	bool	removeItem(int nItemID, int nItemNum);

public:
	uint32		getSize(){return mSize;}
	void		setSize(uint32 nSize){mSize = nSize;}

private:
	uint32		mSize;
};
#endif