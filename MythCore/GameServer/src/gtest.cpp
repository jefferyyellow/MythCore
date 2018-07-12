//#include "gtest/gtest.h"
//#include "objpool.h"
//#include "entityplayer.h"
//#include "objpoolimp.h"
//#define ITEM_ID				133
//#define ITEM_ID_PILE		144
///// 测试obj池释放以后是否还能获得
//TEST(ObjPool, TestFreeObj)
//{
//	CObjPool::createInst();
//	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
//	int nObjId = pPlayer->getObjID();
//	CObjPool::Inst()->free(nObjId);
//	CEntityPlayer* pOldPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(nObjId));
//	EXPECT_EQ(NULL, pOldPlayer);
//}
//
///// 测试多次是否obj
//TEST(ObjPool, TestDoubleFreeObj)
//{
//	CObjPool::createInst();
//	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
//	CObjPoolImp* pPoolImp = CObjPool::Inst()->getPoolImp();
//	int nUsedNum = pPoolImp->mPlayerEntityPool.getUsedNum();
//
//	int nObjId = pPlayer->getObjID();
//	CObjPool::Inst()->free(nObjId);
//	CObjPool::Inst()->free(nObjId);
//	nUsedNum = pPoolImp->mPlayerEntityPool.getUsedNum();
//	EXPECT_EQ(0, nUsedNum);
//}
//
///// 测试背包插入
//TEST(ItemBox, TestBoxInsert)
//{
//	CItemBox tBox;
//	tBox.setSize(BASE_BAG_CAPACITY);
//
//	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutSize = 0;
//
//	tBox.insertItem(ITEM_ID,1, nOutItemIndex, nOutItemNum, nOutSize);
//	CItemObject* pObject = tBox.getItem(0);
//	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
//	EXPECT_EQ(1, pObject->GetItemNum());
//	EXPECT_EQ(0, nOutItemIndex[0]);
//	EXPECT_EQ(1, nOutItemNum[0]);
//
//	tBox.insertItem(ITEM_ID, 1, nOutItemIndex, nOutItemNum, nOutSize);
//	pObject = tBox.getItem(1);
//	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
//	EXPECT_EQ(1, pObject->GetItemNum());
//	EXPECT_EQ(1, nOutItemIndex[0]);
//	EXPECT_EQ(1, nOutItemNum[0]);
//
//	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//	pObject = tBox.getItem(2);
//	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
//	EXPECT_EQ(1, pObject->GetItemNum());
//	EXPECT_EQ(2, nOutItemIndex[0]);
//	EXPECT_EQ(1, nOutItemNum[0]);
//
//	pObject = tBox.getItem(3);
//	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
//	EXPECT_EQ(1, pObject->GetItemNum());
//	EXPECT_EQ(3, nOutItemIndex[1]);
//	EXPECT_EQ(1, nOutItemNum[1]);
//
//	pObject = tBox.getItem(4);
//	EXPECT_EQ(ITEM_ID, pObject->GetItemID());
//	EXPECT_EQ(1, pObject->GetItemNum());
//	EXPECT_EQ(4, nOutItemIndex[2]);
//	EXPECT_EQ(1, nOutItemNum[2]);
//
//	tBox.insertItem(ITEM_ID, 20, nOutItemIndex, nOutItemNum, nOutSize);
//
//}
//
///// 测试背包插入
//TEST(ItemBox, TestBoxInsertPile)
//{
//	CItemBox tBox;
//	tBox.setSize(BASE_BAG_CAPACITY);
//
//	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutSize = 0;
//	
//	tBox.insertItem(ITEM_ID_PILE, 1, nOutItemIndex, nOutItemNum, nOutSize);
//	CItemObject* pObject = tBox.getItem(0);
//	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//	EXPECT_EQ(1, pObject->GetItemNum());
//	EXPECT_EQ(0, nOutItemIndex[0]);
//	EXPECT_EQ(1, nOutItemNum[0]);
//
//	tBox.insertItem(ITEM_ID_PILE, 11, nOutItemIndex, nOutItemNum, nOutSize);
//	pObject = tBox.getItem(0);
//	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//	EXPECT_EQ(10, pObject->GetItemNum());
//	EXPECT_EQ(0, nOutItemIndex[0]);
//	EXPECT_EQ(10, nOutItemNum[0]);
//
//	pObject = tBox.getItem(1);
//	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//	EXPECT_EQ(2, pObject->GetItemNum());
//	EXPECT_EQ(1, nOutItemIndex[1]);
//	EXPECT_EQ(2, nOutItemNum[1]);
//
//	tBox.insertItem(ITEM_ID_PILE, 223, nOutItemIndex, nOutItemNum, nOutSize);
//	pObject = tBox.getItem(23);
//	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//	EXPECT_EQ(5, pObject->GetItemNum());
//	EXPECT_EQ(23, nOutItemIndex[22]);
//
//}
//
///// 测试背包插入
//TEST(ItemBox, TestBoxInsertMixPile)
//{
//	CItemBox tBox;
//	tBox.setSize(BASE_BAG_CAPACITY);
//
//	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutSize = 0;
//
//	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
//	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
//
//	CItemObject* pObject = NULL;
//	for (int i = 0; i < 3; ++ i)
//	{
//		pObject = tBox.getItem(i);
//		EXPECT_EQ(ITEM_ID, pObject->GetItemID());
//		EXPECT_EQ(1, pObject->GetItemNum());
//	}
//
//
//	for (int i = 3; i < 7; ++i)
//	{
//		pObject = tBox.getItem(i);
//		EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//		EXPECT_EQ(10, pObject->GetItemNum());
//	}
//
//
//	for (int i = 7; i < 10; ++i)
//	{
//		pObject = tBox.getItem(i);
//		EXPECT_EQ(ITEM_ID, pObject->GetItemID());
//		EXPECT_EQ(1, pObject->GetItemNum());
//	}
//
//	for (int i = 10; i < 12; ++i)
//	{
//		pObject = tBox.getItem(i);
//		EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//		EXPECT_EQ(10, pObject->GetItemNum());
//	}
//
//	pObject = tBox.getItem(12);
//	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//	EXPECT_EQ(6, pObject->GetItemNum());
//}
//
//TEST(ItemBox, TestBoxRemove)
//{
//	CItemBox tBox;
//	tBox.setSize(BASE_BAG_CAPACITY);
//
//	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutSize = 0;
//
//	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//	CItemObject* pObject = NULL;
//	for (int i = 0; i < 3; ++i)
//	{
//		pObject = tBox.getItem(i);
//		EXPECT_EQ(ITEM_ID, pObject->GetItemID());
//		EXPECT_EQ(1, pObject->GetItemNum());
//	}
//
//	tBox.removeItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//	for (int i = 0; i < 3; ++i)
//	{
//		pObject = tBox.getItem(i);
//		EXPECT_EQ(NULL, pObject);
//	}
//
//
//	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
//	for (int i = 0; i < 3; ++i)
//	{
//		pObject = tBox.getItem(i);
//		EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//		EXPECT_EQ(10, pObject->GetItemNum());
//	}
//	pObject = tBox.getItem(3);
//	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//	EXPECT_EQ(3, pObject->GetItemNum());
//
//	tBox.removeItem(ITEM_ID_PILE, 3, nOutItemIndex, nOutItemNum, nOutSize);
//	pObject = tBox.getItem(0);
//	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//	EXPECT_EQ(7, pObject->GetItemNum());
//
//	tBox.removeItem(ITEM_ID_PILE, 10, nOutItemIndex, nOutItemNum, nOutSize);
//	pObject = tBox.getItem(0);
//	EXPECT_EQ(NULL, pObject);
//	pObject = tBox.getItem(1);
//	EXPECT_EQ(ITEM_ID_PILE, pObject->GetItemID());
//	EXPECT_EQ(7, pObject->GetItemNum());
//
//}
//
//TEST(ItemBox, TestBoxSpace)
//{
//	CItemBox tBox;
//	tBox.setSize(BASE_BAG_CAPACITY);
//
//	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutSize = 0;
//
//	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//
//	int nItemID = ITEM_ID;
//	int nItemNum = BASE_BAG_CAPACITY - 2;
//	bool bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
//	EXPECT_EQ(false, bCheck);
//
//	nItemNum = BASE_BAG_CAPACITY - 3;
//	bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
//	EXPECT_EQ(true, bCheck);
//}
//
//TEST(ItemBox, TestBoxSpacePile)
//{
//	CItemBox tBox;
//	tBox.setSize(BASE_BAG_CAPACITY);
//
//	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutSize = 0;
//
//	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
//	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//
//	int nItemID = ITEM_ID_PILE;
//	int nItemNum = BASE_BAG_CAPACITY * 10 - 92;
//	bool bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
//	EXPECT_EQ(false, bCheck);
//
//	nItemNum = BASE_BAG_CAPACITY * 10 - 93;
//	bCheck = tBox.checkSpace(&nItemID, &nItemNum, 1);
//	EXPECT_EQ(true, bCheck);
//}
//
//TEST(ItemBox, TestBoxSpaceMixPile)
//{
//	CItemBox tBox;
//	tBox.setSize(BASE_BAG_CAPACITY);
//
//	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
//	int nOutSize = 0;
//
//	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//	tBox.insertItem(ITEM_ID_PILE, 33, nOutItemIndex, nOutItemNum, nOutSize);
//	tBox.insertItem(ITEM_ID, 3, nOutItemIndex, nOutItemNum, nOutSize);
//
//
//	int nItemID[] = {ITEM_ID, ITEM_ID_PILE};
//	int nItemNum[] = {3, 478};
//	bool bCheck = tBox.checkSpace(nItemID, nItemNum, 2);
//	EXPECT_EQ(false, bCheck);
//
//	nItemNum[1] = 477;
//	bCheck = tBox.checkSpace(nItemID, nItemNum, 2);
//	EXPECT_EQ(true, bCheck);
//}