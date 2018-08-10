#include "gtest/gtest.h"
#include "objpool.h"
#include "entityplayer.h"
#include "objpoolimp.h"
#include "dirtyword.h"
#include "i18n.h"

/// ≤‚ ‘obj≥ÿ Õ∑≈“‘∫Û «∑Òªπƒ‹ªÒµ√
TEST(ObjPool, TestFreeObj)
{
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	int nObjId = pPlayer->getObjID();
	CObjPool::Inst()->free(nObjId);
	CEntityPlayer* pOldPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(nObjId));
	EXPECT_EQ(NULL, pOldPlayer);
}

/// ≤‚ ‘∂‡¥Œ Õ∑≈obj
TEST(ObjPool, TestDoubleFreeObj)
{
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	CObjPoolImp* pPoolImp = CObjPool::Inst()->getPoolImp();
	int nUsedNum = pPoolImp->mPlayerEntityPool.getUsedNum();

	int nObjId = pPlayer->getObjID();
	CObjPool::Inst()->free(nObjId);
	CObjPool::Inst()->free(nObjId);
	nUsedNum = pPoolImp->mPlayerEntityPool.getUsedNum();
	EXPECT_EQ(0, nUsedNum);
}

TEST(CDirtyWord, checkDirtyWord)
{
	CDirtyWord::Inst()->clear();
	char tBuffer[1024] = {0};
	CI18N::AnsiToUtf8("≈æ≈æ≈æ", tBuffer, sizeof(tBuffer) - 1);
	CDirtyWord::Inst()->addDirtyWord(tBuffer);
	CI18N::AnsiToUtf8("≈æcsole≈æ", tBuffer, sizeof(tBuffer) - 1);
	CDirtyWord::Inst()->addDirtyWord(tBuffer);
	CI18N::AnsiToUtf8("csole≈æ", tBuffer, sizeof(tBuffer) - 1);
	CDirtyWord::Inst()->addDirtyWord(tBuffer);
	CDirtyWord::Inst()->addDirtyWord(".csole.com");

	EXPECT_TRUE(CDirtyWord::Inst()->checkDirtyWord(".csole.com"));
	CI18N::AnsiToUtf8("≈æ≈æ≈æ", tBuffer, sizeof(tBuffer) - 1);
	EXPECT_TRUE(CDirtyWord::Inst()->checkDirtyWord(tBuffer));
	CI18N::AnsiToUtf8("≈æcsole≈æ", tBuffer, sizeof(tBuffer) - 1);
	EXPECT_TRUE(CDirtyWord::Inst()->checkDirtyWord(tBuffer));
	CI18N::AnsiToUtf8("csole≈æ", tBuffer, sizeof(tBuffer) - 1);
	EXPECT_TRUE(CDirtyWord::Inst()->checkDirtyWord(tBuffer));
	CI18N::AnsiToUtf8("π˛π˛π˛", tBuffer, sizeof(tBuffer) - 1);
	EXPECT_FALSE(CDirtyWord::Inst()->checkDirtyWord(tBuffer));
}