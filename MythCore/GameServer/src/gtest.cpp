#include "gtest/gtest.h"
#include "objpool.h"
#include "entityplayer.h"
#include "objpoolimp.h"
#include "mapmodule.h"
#include "mapmamager.h"
#define ITEM_ID				133
#define ITEM_ID_PILE		144
/// ����obj���ͷ��Ժ��Ƿ��ܻ��
TEST(ObjPool, TestFreeObj)
{
	CObjPool::createInst();
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	int nObjId = pPlayer->getObjID();
	CObjPool::Inst()->free(nObjId);
	CEntityPlayer* pOldPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(nObjId));
	EXPECT_EQ(NULL, pOldPlayer);
}

/// ���Զ���ͷ�obj
TEST(ObjPool, TestDoubleFreeObj)
{
	CObjPool::createInst();
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	CObjPoolImp* pPoolImp = CObjPool::Inst()->getPoolImp();
	int nUsedNum = pPoolImp->mPlayerEntityPool.getUsedNum();

	int nObjId = pPlayer->getObjID();
	CObjPool::Inst()->free(nObjId);
	CObjPool::Inst()->free(nObjId);
	nUsedNum = pPoolImp->mPlayerEntityPool.getUsedNum();
	EXPECT_EQ(0, nUsedNum);
}

/// ���Ա�������
TEST(ItemBox, TestBoxInsert)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	tBox.insertItem(ITEM_ID,1, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(0, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

	tBox.insertItem(ITEM_ID, 1, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(1);
	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(1, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

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

/// ���Ա�������
TEST(ItemBox, TestBoxInsertPile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	
	tBox.insertItem(ITEM_ID_PILE, 1, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(1, pObject->GetItemNum());
	EXPECT_EQ(0, nOutItemIndex[0]);
	EXPECT_EQ(1, nOutItemNum[0]);

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

	tBox.insertItem(ITEM_ID_PILE, 223, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(23);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(5, pObject->GetItemNum());
	EXPECT_EQ(23, nOutItemIndex[22]);

}

/// ���Ա�������
TEST(ItemBox, TestBoxInsertMixPile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);

	CItemObject* pObject = NULL;
	for (int i = 0; i < 3; ++ i)
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

TEST(ItemBox, TestBoxRemove)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	CItemObject* pObject = NULL;
	for (int i = 0; i < 3; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(ITEM_ID, pObject->GetItemID());
		EXPECT_EQ(1, pObject->GetItemNum());
	}

	tBox.removeItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	for (int i = 0; i < 3; ++i)
	{
		pObject = tBox.getItem(i);
		EXPECT_EQ(NULL, pObject);
	}


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

	tBox.removeItem(ITEM_ID_PILE, 3, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(0);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(7, pObject->GetItemNum());

	tBox.removeItem(ITEM_ID_PILE, 10, nOutItemIndex, nOutItemNum, nOutSize);
	pObject = tBox.getItem(0);
	EXPECT_EQ(NULL, pObject);
	pObject = tBox.getItem(1);
	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
	EXPECT_EQ(7, pObject->GetItemNum());

}

TEST(ItemBox, TestBoxSpace)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);

	int nItemID = ITEM_ID;
	int nItemNum = BASE_BAG_CAPACITY - 2;
	bool bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(false, bCheck);

	nItemNum = BASE_BAG_CAPACITY - 3;
	bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(true, bCheck);
}

TEST(ItemBox, TestBoxSpacePile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);

	int nItemID = ITEM_ID_PILE;
	int nItemNum = BASE_BAG_CAPACITY * 10 - 92;
	bool bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(false, bCheck);

	nItemNum = BASE_BAG_CAPACITY * 10 - 93;
	bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
	EXPECT_EQ(true, bCheck);
}

TEST(ItemBox, TestBoxSpaceMixPile)
{
	CItemBox tBox;
	tBox.setSize(BASE_BAG_CAPACITY);

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);


	int nItemID[] = {ITEM_ID, ITEM_ID_PILE};
	int nItemNum[] = {3, 478};
	bool bCheck = tBox.checkSpace(nItemID, nItemNum, 2);
	EXPECT_EQ(false, bCheck);

	nItemNum[1] = 477;
	bCheck = tBox.checkSpace(nItemID, nItemNum, 2);
	EXPECT_EQ(true, bCheck);
}

TEST(MapModule, createMapFromConfig)
{
	const CMap* pMap = CMapModule::Inst()->createMapFromConfig(1, 1, 1);
	EXPECT_NE(nullptr, pMap);
	const CMap* pGetMap = CMapManager::Inst()->getMap(1,1,1);
	EXPECT_EQ(pMap, pGetMap);
	CMapManager::Inst()->destroyMap(1,1,1);
	pGetMap = CMapManager::Inst()->getMap(1, 1, 1);
	EXPECT_EQ(nullptr, pGetMap);
}

TEST(MapModule, EntityAlloc)
{
	CMap* pMap = CMapModule::Inst()->createMapFromConfig(1, 1, 1);
	EXPECT_NE(nullptr, pMap);
	CMapCell* pMapCell = pMap->getMapCell(0, 0);
	EXPECT_NE(nullptr, pMapCell);
	int nEntityNum = pMapCell->getEntityNum();
	EXPECT_EQ(0, nEntityNum);
	pMapCell->pushEntity(1);
	nEntityNum = pMapCell->getEntityNum();
	EXPECT_EQ(1, nEntityNum);
	EXPECT_TRUE(pMapCell->checkEntity(1));
	EXPECT_FALSE(pMapCell->checkEntity(2));

	pMapCell->pushEntity(2);

	nEntityNum = pMapCell->getEntityNum();
	EXPECT_EQ(2, nEntityNum);

	EXPECT_TRUE(pMapCell->checkEntity(1));
	EXPECT_TRUE(pMapCell->checkEntity(2));

	pMapCell->pushEntity(3);
	pMapCell->pushEntity(4);

	nEntityNum = pMapCell->getEntityNum();
	EXPECT_EQ(4, nEntityNum);

	pMapCell->removeEntity(1);
	pMapCell->removeEntity(2);
	pMapCell->removeEntity(3);
	pMapCell->removeEntity(4);
	nEntityNum = pMapCell->getEntityNum();
	EXPECT_EQ(0, nEntityNum);
	EXPECT_FALSE(pMapCell->checkEntity(1));
	EXPECT_FALSE(pMapCell->checkEntity(2));

}