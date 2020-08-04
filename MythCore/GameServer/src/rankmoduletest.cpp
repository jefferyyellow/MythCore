#include "gtest/gtest.h"
#include "entityplayer.h"
#include "rankmodule.h"
#include "timemanager.h"
TEST(ItemBox, updateRoleSame)
{
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer->setLevel(10);
	pPlayer->setRoleID(0);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer, pPlayer->getLevel());

	CTimeManager::Inst()->setCurrTime(CTimeManager::Inst()->getCurrTime() + 1);
	CEntityPlayer* pPlayer1 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer1->setLevel(10);
	pPlayer1->setRoleID(1);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer1, pPlayer1->getLevel());
	CTimeManager::Inst()->setCurrTime(CTimeManager::Inst()->getCurrTime() + 1);

	CEntityPlayer* pPlayer2 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer2->setLevel(10);
	pPlayer2->setRoleID(2);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer2, pPlayer2->getLevel());

	EXPECT_EQ(0, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer->getRoleID()));
	EXPECT_EQ(1, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer1->getRoleID()));
	EXPECT_EQ(2, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer2->getRoleID()));
}

TEST(ItemBox, updateRoleRank)
{
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer->setLevel(10);
	pPlayer->setRoleID(0);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer, pPlayer->getLevel());

	CEntityPlayer* pPlayer1 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer1->setLevel(15);
	pPlayer1->setRoleID(1);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer1, pPlayer1->getLevel());

	CEntityPlayer* pPlayer2 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer2->setLevel(5);
	pPlayer2->setRoleID(2);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer2, pPlayer2->getLevel());

	EXPECT_EQ(1, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer->getRoleID()));
	EXPECT_EQ(0, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer1->getRoleID()));
	EXPECT_EQ(2, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer2->getRoleID()));


}

TEST(ItemBox, updateRoleRank2)
{
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer->setLevel(20);
	pPlayer->setRoleID(0);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer, pPlayer->getLevel());

	CEntityPlayer* pPlayer1 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer1->setLevel(15);
	pPlayer1->setRoleID(1);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer1, pPlayer1->getLevel());

	CEntityPlayer* pPlayer2 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer2->setLevel(19);
	pPlayer2->setRoleID(2);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer2, pPlayer2->getLevel());

	EXPECT_EQ(0, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer->getRoleID()));
	EXPECT_EQ(2, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer1->getRoleID()));
	EXPECT_EQ(1, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer2->getRoleID()));

	pPlayer->setLevel(25);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer, pPlayer->getLevel());
	EXPECT_EQ(0, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer->getRoleID()));
	EXPECT_EQ(2, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer1->getRoleID()));
	EXPECT_EQ(1, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer2->getRoleID()));


	pPlayer2->setLevel(24);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer2, pPlayer2->getLevel());
	EXPECT_EQ(0, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer->getRoleID()));
	EXPECT_EQ(2, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer1->getRoleID()));
	EXPECT_EQ(1, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer2->getRoleID()));
}

TEST(ItemBox, updateRoleRankDesc)
{
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer->setLevel(90);
	pPlayer->setRoleID(0);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer, pPlayer->getLevel());

	CEntityPlayer* pPlayer1 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer1->setLevel(80);
	pPlayer1->setRoleID(1);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer1, pPlayer1->getLevel());

	CEntityPlayer* pPlayer2 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer2->setLevel(70);
	pPlayer2->setRoleID(2);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer2, pPlayer2->getLevel());


	CEntityPlayer* pPlayer3 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer3->setLevel(60);
	pPlayer3->setRoleID(3);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer3, pPlayer3->getLevel());


	EXPECT_EQ(0, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer->getRoleID()));
	EXPECT_EQ(1, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer1->getRoleID()));
	EXPECT_EQ(2, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer2->getRoleID()));
	EXPECT_EQ(3, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer3->getRoleID()));
}

TEST(ItemBox, updateRoleRankAsc)
{
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer->setLevel(60);
	pPlayer->setRoleID(0);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer, pPlayer->getLevel());

	CEntityPlayer* pPlayer1 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer1->setLevel(70);
	pPlayer1->setRoleID(1);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer1, pPlayer1->getLevel());

	CEntityPlayer* pPlayer2 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer2->setLevel(80);
	pPlayer2->setRoleID(2);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer2, pPlayer2->getLevel());


	CEntityPlayer* pPlayer3 = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	pPlayer3->setLevel(90);
	pPlayer3->setRoleID(3);
	CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer3, pPlayer3->getLevel());


	EXPECT_EQ(3, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer->getRoleID()));
	EXPECT_EQ(2, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer1->getRoleID()));
	EXPECT_EQ(1, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer2->getRoleID()));
	EXPECT_EQ(0, CRankModule::Inst()->getRoleRank(emRankType_Level, pPlayer3->getRoleID()));
}

TEST(ItemBox, updateRoleRankRand)
{
	int nMaxLevel = 0;
	int nMaxLevlePlayerID = -1;

	int nSecLevel = 0;
	int nSecLevlePlayerID = -1;
	for (int i = 0; i < 100; ++i)
	{
		CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
		pPlayer->setLevel(RAND(100));
		pPlayer->setRoleID(i);
		if (pPlayer->getLevel() > nMaxLevel)
		{
			nSecLevel = nMaxLevel;
			nSecLevlePlayerID = nMaxLevlePlayerID;

			nMaxLevel = pPlayer->getLevel();
			nMaxLevlePlayerID = i;
		}
		else if (pPlayer->getLevel() > nSecLevel)
		{
			nSecLevel = pPlayer->getLevel();
			nSecLevlePlayerID = i;
		}
		CTimeManager::Inst()->setCurrTime(CTimeManager::Inst()->getCurrTime() + 1);
		CRankModule::Inst()->updateRoleRank(emRankType_Level, pPlayer, pPlayer->getLevel());
	}

	EXPECT_EQ(0, CRankModule::Inst()->getRoleRank(emRankType_Level, nMaxLevlePlayerID));
	EXPECT_EQ(1, CRankModule::Inst()->getRoleRank(emRankType_Level, nSecLevlePlayerID));
}