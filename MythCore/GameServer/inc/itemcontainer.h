#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__
#define MAX_INSERT_TYPE_NUM 16
#include "itemlist.h"
#define  MAX_CONTAINER_ITEM_NUM 100
class CItemObject;
class CItemBox : public CItemList < MAX_CONTAINER_ITEM_NUM >
{
public:
	CItemBox()
	{
		init();
	}
	~CItemBox()
	{

	}

	void init()
	{
        mSize = 0;
	}

public:
	/// 是否有足够的空间放下这一堆道具
	bool	checkSpace(int* pItemID, int* pNumber, int nSize);
	// 是否有足够的空间放指定的道具
	bool	checkSpace(int nItemID, int nNumber);
	// 插入道具
	int		insertItem(int nItemID, int nItemNum, int *pOutIndex, int *pOutNumber, int &rOutLen);
	/// 背包里是否有足够数量的道具
	bool	checkEnough(int nItemID, int nItemNum);
	/// 删除道具
	void	removeItem(int nItemID, int nItemNum, int *pOutIndex, int *pOutNumber, int &rOutLen);
	/// 删除道具
	int		removeItem(unsigned int nIndex, int nNum);
	/// 拥有道具的数目
	int		hasItem(int nItemID);
	/// 序列化
	void	createToPB(PBItemList* pbItemList);
	/// 反序列化
	void	setFromPB(PBItemList* pbItemList);
	/// 得到空格子的数目
	int		getBlankSpace();
	/// 移动道具
	int		move(unsigned int nSrcIndex, CItemBox& rDestBox, int nDestIndex, int nNum);
	/// 检查索引是否非法
	bool	checkIndexInvalid(uint nIndex);
public:
	/// autocode don't edit!!!
    int getSize(){ return mSize;}
    void setSize(int value){ mSize = value;}
	/// end autocode

private:
	int		mSize;
};
#endif