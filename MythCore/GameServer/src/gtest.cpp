#include "gtest/gtest.h"
#include "objpool.h"
#include "entityplayer.h"
#include "objpoolimp.h"

/// 测试obj池释放以后是否还能获得
TEST(ObjPool, TestFreeObj)
{
	CObjPool::createInst();
	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->allocObj(emObjType_Entity_Player));
	int nObjId = pPlayer->getObjID();
	CObjPool::Inst()->free(nObjId);
	CEntityPlayer* pOldPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(nObjId));
	EXPECT_EQ(NULL, pOldPlayer);
}

/// 测试多次释放obj
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