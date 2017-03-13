#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__
#define MAX_INSERT_TYPE_NUM 16
#include "itemlist.h"
#define  MAX_CONTAINER_ITEM_NUM 100
class CItemContainer : public CItemList < MAX_CONTAINER_ITEM_NUM >
{
public:

public:
	bool	checkSpace(int* pItemID, int* pNumber, int nSize);
	bool	checkSpace(int nItemID, int nNumber);
	int		insertItem(int nItemID, int nItemNum, int *pOutIndex, int *pOutNumber, int &rOutLen);

public:
	uint32		getSize(){return mSize;}
	void		setSize(uint32 nSize){mSize = nSize;}

private:
	uint32		mSize;
};
#endif