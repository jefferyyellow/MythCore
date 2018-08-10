#include "gtest/gtest.h"
#include "mapmodule.h"
#include "mapmamager.h"
#include "entitycreator.h"
#define OGRE_ID 2000

TEST(MapModule, createMapFromConfig)
{
	const CMap* pMap = CMapModule::Inst()->createMapFromConfig(1, 1, 1);
	EXPECT_NE((void*)0, pMap);
	const CMap* pGetMap = CMapManager::Inst()->getMap(1, 1, 1);
	EXPECT_EQ(pMap, pGetMap);
	CMapManager::Inst()->destroyMap(1, 1, 1);
	pGetMap = CMapManager::Inst()->getMap(1, 1, 1);
	EXPECT_EQ((void*)0, pGetMap);
}

TEST(MapModule, EntityAlloc)
{
	CMap* pMap = CMapModule::Inst()->createMapFromConfig(1, 1, 1);
	EXPECT_NE((void*)0, pMap);
	CMapCell* pMapCell = pMap->getMapCell(0, 0);
	EXPECT_NE((void*)0, pMapCell);
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
	CMapManager::Inst()->destroyMap(1, 1, 1);
}

TEST(MapModule, VisibleRect)
{
	CMap* pMap = CMapModule::Inst()->createMapFromConfig(1, 1, 1);

	CEntityPlayer tPlayer;
	tPlayer.setPos(0,0);

	int nLength = pMap->getLength();
	int nWidth = pMap->getWidth();

	CMythRect tRect;
	pMap->getVisibleRect(&tPlayer, tRect);
	EXPECT_EQ(0, tRect.topLeft().mX);
	EXPECT_EQ(0, tRect.topLeft().mY);
	EXPECT_EQ(VISIBLE_RADIUS_X, tRect.bottomRight().mX);
	EXPECT_EQ(VISIBLE_RADIUS_Y, tRect.bottomRight().mY);

	tPlayer.setPos(VISIBLE_RADIUS_X + 1, VISIBLE_RADIUS_Y + 1);
	pMap->getVisibleRect(&tPlayer, tRect);
	EXPECT_EQ(1, tRect.topLeft().mX);
	EXPECT_EQ(1, tRect.topLeft().mY);
	EXPECT_EQ(VISIBLE_RADIUS_X * 2 + 1, tRect.bottomRight().mX);
	EXPECT_EQ(VISIBLE_RADIUS_Y * 2 + 1, tRect.bottomRight().mY);

	tPlayer.setPos(nLength, nWidth);
	pMap->getVisibleRect(&tPlayer, tRect);
	EXPECT_EQ(nLength - VISIBLE_RADIUS_X, tRect.topLeft().mX);
	EXPECT_EQ(nWidth - VISIBLE_RADIUS_Y, tRect.topLeft().mY);
	EXPECT_EQ(nLength - 1, tRect.bottomRight().mX);
	EXPECT_EQ(nWidth - 1, tRect.bottomRight().mY);
	CMapManager::Inst()->destroyMap(1, 1, 1);
}


TEST(MapModule, createEntity)
{
	CMap* pMap = CMapModule::Inst()->createMapFromConfig(1, 1, 1);
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer->setPos(0, 0);

	pMap->createPlayer(pPlayer);
	CMapCell* pMapCell = pMap->getMapCell(0, 0);
	EXPECT_EQ(1, pMapCell->getEntityNum());
	EXPECT_TRUE(pMapCell->checkEntity(pPlayer->getObjID()));

	COgreCreator tOgreCreator;
	tOgreCreator.setType(emEntityType_Ogre);
	tOgreCreator.mTempID = OGRE_ID;
	tOgreCreator.mLineID = 1;
	tOgreCreator.mMapID = 1;
	tOgreCreator.mMapIndex = 1;
	tOgreCreator.mPos = CMythPoint(0,0);
	CEntityOgre* pOgre = static_cast<CEntityOgre*>(pMap->createEntity(&tOgreCreator));
	EXPECT_NE((void*)0, pOgre);
	EXPECT_EQ(2, pMapCell->getEntityNum());
	EXPECT_TRUE(pMapCell->checkEntity(pPlayer->getObjID()));
	EXPECT_TRUE(pMapCell->checkEntity(pOgre->getObjID()));


	CMythPoint tPoint(11,11);
	pMap->onEntityMove(pPlayer, tPoint);

	pMapCell = pMap->getMapCell(0, 0);
	EXPECT_EQ(1, pMapCell->getEntityNum());
	EXPECT_FALSE(pMapCell->checkEntity(pPlayer->getObjID()));
	EXPECT_TRUE(pMapCell->checkEntity(pOgre->getObjID()));


	pMapCell = pMap->getMapCell(11, 11);
	EXPECT_EQ(1, pMapCell->getEntityNum());
	EXPECT_TRUE(pMapCell->checkEntity(pPlayer->getObjID()));
	EXPECT_FALSE(pMapCell->checkEntity(pOgre->getObjID()));

	pMap->onEntityMove(pOgre, tPoint);

	pMapCell = pMap->getMapCell(0, 0);
	EXPECT_EQ(0, pMapCell->getEntityNum());
	EXPECT_FALSE(pMapCell->checkEntity(pPlayer->getObjID()));
	EXPECT_FALSE(pMapCell->checkEntity(pOgre->getObjID()));

	pMapCell = pMap->getMapCell(11, 11);
	EXPECT_EQ(2, pMapCell->getEntityNum());
	EXPECT_TRUE(pMapCell->checkEntity(pPlayer->getObjID()));
	EXPECT_TRUE(pMapCell->checkEntity(pOgre->getObjID()));


	pMap->destroyPlayer(pPlayer);
	pMapCell = pMap->getMapCell(11, 11);
	EXPECT_EQ(1, pMapCell->getEntityNum());
	EXPECT_FALSE(pMapCell->checkEntity(pPlayer->getObjID()));
	EXPECT_TRUE(pMapCell->checkEntity(pOgre->getObjID()));


	pMap->destroyEntity(pOgre);
	EXPECT_EQ(0, pMapCell->getEntityNum());
	EXPECT_FALSE(pMapCell->checkEntity(pPlayer->getObjID()));
	EXPECT_FALSE(pMapCell->checkEntity(pOgre->getObjID()));

	CMapManager::Inst()->destroyMap(1, 1, 1);
}