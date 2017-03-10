#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__
#define MAX_INSERT_TYPE_NUM 16
#include "itemlist.h"
class CItemContainer : public CItemList < 1000 >
{
public:

public:
	bool checkSpace(int* pItemID, int* pNumber, int nSize);
	bool checkSpace(int nItemID, int nNumber);

public:
	uint32		getSize(){return mSize;}
	void		setSize(uint32 nSize){mSize = nSize;}

private:
	uint32		mSize;
};
#endif