#include "gtest/gtest.h"
#include "objpoolimp.h"
#include "errcode.h"
#include "testtype.h"

/// 测试背包插入（不可堆叠道具）
TEST(ItemBox, TestBoxInsert)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	// 插入1个不可以堆叠道具
	tBox.insertItem(ITEM_ID, 1, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(0, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

	// 再次插入1个不可以堆叠的道具
	tBox.insertItem(ITEM_ID, 1, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(1);
	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(1, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

	// 第三次插入3个不可堆叠的道具
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(2);
	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(2, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

	pObject = tBox.getItem(3);
	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(3, nOutItemIndex[1]);
	EXPECT_EQ(1, nOutItemNum[1]);

	pObject = tBox.getItem(4);
	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(4, nOutItemIndex[2]);
	EXPECT_EQ(1, nOutItemNum[2]);

	tBox.insertItem(ITEM_ID, 20, nOutItemIndex, nOutItemNum, nOutSize);

}

/// 测试背包插入（可堆叠道具）
TEST(ItemBox, TestBoxInsertPile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// 插入1个可以堆叠道具
	tBox.insertItem(ITEM_ID_PILE, 1, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(0, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

	// 插入11个可以堆叠道具
	tBox.insertItem(ITEM_ID_PILE, 11, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(10, pObject->GetItemNum());
	EXPECT_EQ(0, nOutItemIndex[0]);
	EXPECT_EQ(10, nOutItemNum[0]);

	pObject = tBox.getItem(1);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(2, pObject->GetItemNum());
	EXPECT_EQ(1, nOutItemIndex[1]);
	EXPECT_EQ(2, nOutItemNum[1]);

	// 插入223个可以堆叠道具
	tBox.insertItem(ITEM_ID_PILE, 223, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(23);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(5, pObject->GetItemNum());
	EXPECT_EQ(23, nOutItemIndex[22]);

}

/// 测试背包插入（可堆叠的，不可堆叠的交叉插入）
TEST(ItemBox, TestBoxInsertMixPile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入33个可以堆叠的
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入3个不可以堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入33个可以堆叠的
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);

	CItemObject* pObject = NULL;
	for (int i = 0; i < 3; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(ITEM_ID, pObject->GetItemID());
		EXPECT_EQ(1, pObject->GetItemNum());
	}


	for (int i = 3; i < 7; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
		EXPECT_EQ(10, pObject->GetItemNum());
	}


	for (int i = 7; i < 10; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(ITEM_ID, pObject->GetItemID());
		EXPECT_EQ(1, pObject->GetItemNum());
	}

	for (int i = 10; i < 12; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
		EXPECT_EQ(10, pObject->GetItemNum());
	}

	pObject = tBox.getItem(12);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(6, pObject->GetItemNum());
}

// 测试背包删除
TEST(ItemBox, TestBoxRemove)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = NULL;
	for (int i = 0; i < 3; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(ITEM_ID, pObject->GetItemID());
		EXPECT_EQ(1, pObject->GetItemNum());
	}

	// 移除3个不可堆叠的
	tBox.removeItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	for (int i = 0; i < 3; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(NULL, pObject);
	}

	// 插入33个可以堆叠的
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	for (int i = 0; i < 3; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
		EXPECT_EQ(10, pObject->GetItemNum());
	}
	pObject = tBox.getItem(3);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(3, pObject->GetItemNum());
	// 移除3个可以堆叠的
	tBox.removeItem(ITEM_ID_PILE, 3, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(7, pObject->GetItemNum());
	// 移除10个可以堆叠的
	tBox.removeItem(ITEM_ID_PILE, 10, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(0);
	EXPECT_EQ(NULL, pObject);
	pObject = tBox.getItem(1);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(7, pObject->GetItemNum());
	// 移除20个可以堆叠的
	tBox.removeItem(ITEM_ID_PILE, 20, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(0);
	EXPECT_EQ(NULL, pObject);
	pObject = tBox.getItem(1);
	EXPECT_EQ(NULL, pObject);
	pObject = tBox.getItem(2);
	EXPECT_EQ(NULL, pObject);
	pObject = tBox.getItem(3);
	EXPECT_EQ(NULL, pObject);
}

// 测试背包删除
TEST(ItemBox, TestBoxRemove2)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入33个可以堆叠的
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入3个不可以堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = NULL;

	tBox.removeItem(0, 1);
	pObject = tBox.getItem(0);
	EXPECT_EQ(NULL, pObject);

	pObject = tBox.getItem(1);
	EXPECT_EQ(1, pObject->GetItemNum());

	tBox.removeItem(2, 1);
	pObject = tBox.getItem(2);
	EXPECT_EQ(NULL, pObject);


	tBox.removeItem(3, 1);
	pObject = tBox.getItem(3);
	EXPECT_EQ(9, pObject->GetItemNum());

	tBox.removeItem(4, 10);
	pObject = tBox.getItem(4);
	EXPECT_EQ(NULL, pObject);

	EXPECT_EQ(ERR_ITEM_INDEX_NUM_NOT_ENOUGH, tBox.removeItem(5, 11));
}

// 测试检查背包空间（不可堆叠）
TEST(ItemBox, TestBoxSpace)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);

	int nItemID = ITEM_ID;
	int nItemNum = BASE_BAG_CAPACITY - 2;
	bool bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(false, bCheck);

	nItemNum = BASE_BAG_CAPACITY - 3;
	bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(true, bCheck);
}

// 测试检查背包空间（可堆叠）
TEST(ItemBox, TestBoxSpacePile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入33个可堆叠的
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);

	int nItemID = ITEM_ID_PILE;
	int nItemNum = BASE_BAG_CAPACITY * 10 - 92;
	bool bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(false, bCheck);

	nItemNum = BASE_BAG_CAPACITY * 10 - 93;
	bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(true, bCheck);
}

// 测试检查背包空间(不可堆叠和可堆叠混合的检查）
TEST(ItemBox, TestBoxSpaceMixPile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入33个可堆叠的
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);


	int nItemID[] = { ITEM_ID, ITEM_ID_PILE };
	int nItemNum[] = { 3, 478 };
	bool bCheck = tBox.checkSpace(nItemID, nItemNum, 2);
	EXPECT_EQ(false, bCheck);

	nItemNum[1] = 477;
	bCheck = tBox.checkSpace(nItemID, nItemNum, 2);
	EXPECT_EQ(true, bCheck);
}

// 检查道具是否足够
TEST(ItemBox, TestCheckItemEnough)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入33个可堆叠的
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);

	EXPECT_FALSE(tBox.checkEnough(ITEM_ID, 7));
	EXPECT_TRUE(tBox.checkEnough(ITEM_ID, 6));

	EXPECT_FALSE(tBox.checkEnough(ITEM_ID_PILE, 34));
	EXPECT_TRUE(tBox.checkEnough(ITEM_ID_PILE,33));
}

// 测试得到指定道具数目的接口
TEST(ItemBox, TestHasItem)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入33个可堆叠的
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入3个不可堆叠的
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// 插入33个可堆叠的
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);

	EXPECT_EQ(6, tBox.hasItem(ITEM_ID));
	EXPECT_EQ(66, tBox.hasItem(ITEM_ID_PILE));
}