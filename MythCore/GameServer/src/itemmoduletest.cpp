#include "gtest/gtest.h"
#include "objpoolimp.h"
#include "errcode.h"
#include "testtype.h"

/// ���Ա������루���ɶѵ����ߣ�
TEST(ItemBox, TestBoxInsert)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	// ����1�������Զѵ�����
	tBox.insertItem(ITEM_ID, 1, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(0, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

	// �ٴβ���1�������Զѵ��ĵ���
	tBox.insertItem(ITEM_ID, 1, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(1);
	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(1, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

	// �����β���3�����ɶѵ��ĵ���
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

/// ���Ա������루�ɶѵ����ߣ�
TEST(ItemBox, TestBoxInsertPile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// ����1�����Զѵ�����
	tBox.insertItem(ITEM_ID_PILE, 1, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(0, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

	// ����11�����Զѵ�����
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

	// ����223�����Զѵ�����
	tBox.insertItem(ITEM_ID_PILE, 223, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(23);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(5, pObject->GetItemNum());
	EXPECT_EQ(23, nOutItemIndex[22]);

}

/// ���Ա������루�ɶѵ��ģ����ɶѵ��Ľ�����룩
TEST(ItemBox, TestBoxInsertMixPile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// ����33�����Զѵ���
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// ����3�������Զѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// ����33�����Զѵ���
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

// ���Ա���ɾ��
TEST(ItemBox, TestBoxRemove)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = NULL;
	for (int i = 0; i < 3; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(ITEM_ID, pObject->GetItemID());
		EXPECT_EQ(1, pObject->GetItemNum());
	}

	// �Ƴ�3�����ɶѵ���
	tBox.removeItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	for (int i = 0; i < 3; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(NULL, pObject);
	}

	// ����33�����Զѵ���
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
	// �Ƴ�3�����Զѵ���
	tBox.removeItem(ITEM_ID_PILE, 3, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(7, pObject->GetItemNum());
	// �Ƴ�10�����Զѵ���
	tBox.removeItem(ITEM_ID_PILE, 10, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(0);
	EXPECT_EQ(NULL, pObject);
	pObject = tBox.getItem(1);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(7, pObject->GetItemNum());
	// �Ƴ�20�����Զѵ���
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

// ���Ա���ɾ��
TEST(ItemBox, TestBoxRemove2)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// ����33�����Զѵ���
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// ����3�������Զѵ���
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

// ���Լ�鱳���ռ䣨���ɶѵ���
TEST(ItemBox, TestBoxSpace)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);

	int nItemID = ITEM_ID;
	int nItemNum = BASE_BAG_CAPACITY - 2;
	bool bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(false, bCheck);

	nItemNum = BASE_BAG_CAPACITY - 3;
	bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(true, bCheck);
}

// ���Լ�鱳���ռ䣨�ɶѵ���
TEST(ItemBox, TestBoxSpacePile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// ����33���ɶѵ���
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);

	int nItemID = ITEM_ID_PILE;
	int nItemNum = BASE_BAG_CAPACITY * 10 - 92;
	bool bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(false, bCheck);

	nItemNum = BASE_BAG_CAPACITY * 10 - 93;
	bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(true, bCheck);
}

// ���Լ�鱳���ռ�(���ɶѵ��Ϳɶѵ���ϵļ�飩
TEST(ItemBox, TestBoxSpaceMixPile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// ����33���ɶѵ���
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);


	int nItemID[] = { ITEM_ID, ITEM_ID_PILE };
	int nItemNum[] = { 3, 478 };
	bool bCheck = tBox.checkSpace(nItemID, nItemNum, 2);
	EXPECT_EQ(false, bCheck);

	nItemNum[1] = 477;
	bCheck = tBox.checkSpace(nItemID, nItemNum, 2);
	EXPECT_EQ(true, bCheck);
}

// �������Ƿ��㹻
TEST(ItemBox, TestCheckItemEnough)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// ����33���ɶѵ���
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);

	EXPECT_FALSE(tBox.checkEnough(ITEM_ID, 7));
	EXPECT_TRUE(tBox.checkEnough(ITEM_ID, 6));

	EXPECT_FALSE(tBox.checkEnough(ITEM_ID_PILE, 34));
	EXPECT_TRUE(tBox.checkEnough(ITEM_ID_PILE,33));
}

// ���Եõ�ָ��������Ŀ�Ľӿ�
TEST(ItemBox, TestHasItem)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// ����33���ɶѵ���
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	// ����3�����ɶѵ���
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	// ����33���ɶѵ���
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);

	EXPECT_EQ(6, tBox.hasItem(ITEM_ID));
	EXPECT_EQ(66, tBox.hasItem(ITEM_ID_PILE));
}