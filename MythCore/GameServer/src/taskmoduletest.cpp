#include "gtest/gtest.h"
#include "entityplayer.h"
#include "taskunit.h"
#include "taskmodule.h"
#include "errcode.h"
#include "testtype.h"
TEST(TaskModule, SetTaskConfig)
{
	CTaskModule::Inst()->clear();

	CTaskConfig* pNewConfig = new CTaskConfig;
	EXPECT_NE((void*)0, pNewConfig);

	pNewConfig->mId = 1;
	pNewConfig->mType = emTaskType_Main;
	pNewConfig->mAcceptMapId = 1;
	pNewConfig->mSubmitMapId = 2;
	pNewConfig->mAcceptNpc = TASK_NPC_ID;
	pNewConfig->mSubmitNpc = TASK_NPC_ID;

	// 接受条件：前置任务
	CTaskCondition tAccpetCond;
	tAccpetCond.mType = 1;
	tAccpetCond.mParam[0] =1999;
	pNewConfig->mAcceptCondition.push_back(tAccpetCond);

	// 接受条件：等级
	CTaskCondition tAccpetCond2;
	tAccpetCond2.mType = 2;
	tAccpetCond2.mParam[0] = 10;
	tAccpetCond2.mParam[1] = 10;
	pNewConfig->mAcceptCondition.push_back(tAccpetCond2);

	// 完成条件：拥有道具
	CTaskCondition tCompleteCond;
	tCompleteCond.mType = 2;
	tCompleteCond.mParam[0] = ITEM_ID_PILE;
	tCompleteCond.mParam[1] = 25;
	pNewConfig->mCompleteCondition.push_back(tCompleteCond);

	// 完成条件：杀怪计数
	CTaskCondition tCompleteCond2;
	tCompleteCond2.mType = 1;
	tCompleteCond2.mParam[0] = TASK_OGRE_ID;
	tCompleteCond2.mParam[1] = 10;
	pNewConfig->mCompleteCondition.push_back(tCompleteCond2);

	// 完成条件：等级
	CTaskCondition tCompleteCond3;
	tCompleteCond3.mType = 3;
	tCompleteCond3.mParam[0] = 10;
	pNewConfig->mCompleteCondition.push_back(tCompleteCond3);

	// 完成奖励：道具
	CTaskReward tReward;
	tReward.mType = 1;
	tReward.mParam[0] = 11015;
	tReward.mParam[1] = 20;
	pNewConfig->mPrizeList.push_back(tReward);

	// 完成奖励：道具
	CTaskReward tReward2;
	tReward2.mType = 1;
	tReward2.mParam[0] = 10001;
	tReward2.mParam[1] = 2;
	pNewConfig->mPrizeList.push_back(tReward2);
	
	CTaskModule::Inst()->setTaskConfig(pNewConfig->mId, pNewConfig);
}

// 检查任务是否完成
TEST(TaskModule, checkTaskComplete)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	rTaskUnit.setTaskComplete(1);
	EXPECT_FALSE(rTaskUnit.checkTaskComplete(0));
	EXPECT_TRUE(rTaskUnit.checkTaskComplete(1));
	EXPECT_FALSE(rTaskUnit.checkTaskComplete(2));
}

// 检查接受任务条件
TEST(TaskModule, checkAcceptCondition)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	// 检查前置任务完成
	tEntityPlayer.setLevel(9);
	EXPECT_EQ(ERROR_TASK_PRE_TASK_NOT_COMPLETE, rTaskUnit.checkAcceptTask(1));
	tEntityPlayer.setLevel(10);
	EXPECT_EQ(ERROR_TASK_PRE_TASK_NOT_COMPLETE, rTaskUnit.checkAcceptTask(1));

	// 检查任务等级不够
	rTaskUnit.setTaskComplete(1999);
	tEntityPlayer.setLevel(9);
	EXPECT_EQ(ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH, rTaskUnit.checkAcceptTask(1));
	tEntityPlayer.setLevel(10);
	EXPECT_EQ(0, rTaskUnit.checkAcceptTask(1));
	tEntityPlayer.setLevel(11);
	EXPECT_EQ(0, rTaskUnit.checkAcceptTask(1));
}

// 检查接受任务
TEST(TaskModule, checkAcceptTask)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	// 已经接受了1
	rTaskUnit.pushBackPlayerTask(1);
	EXPECT_EQ(ERROR_TASK_PLAYER_ALREADY_ACCEPT, rTaskUnit.checkAcceptTask(1));

	rTaskUnit.removeTask(1);
	// 已经完成了1
	rTaskUnit.setTaskComplete(1);
	EXPECT_EQ(ERROR_TASK_PLAYER_ALREADY_COMPLETE, rTaskUnit.checkAcceptTask(1));

	// 没有对应的配置
	EXPECT_EQ(ERROR_TASK_CONFIG_IS_NULL, rTaskUnit.checkAcceptTask(2));
}

// 坚持玩家身上的任务的增，删，查
TEST(TaskModule, pushBackPlayerTask)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	
	// 只插入了1，没有2，3
	rTaskUnit.pushBackPlayerTask(1);
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_NE((void*)0, rTaskUnit.getPlayerTask(1));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_EQ((void*)0, rTaskUnit.getPlayerTask(2));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(3));
	EXPECT_EQ((void*)0, rTaskUnit.getPlayerTask(3));

	// 只插入了1，2，没有3
	rTaskUnit.pushBackPlayerTask(2);
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(3));
	
	// 插入了1，2，3
	rTaskUnit.pushBackPlayerTask(3);
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_NE((void*)0, rTaskUnit.getPlayerTask(1));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_NE((void*)0, rTaskUnit.getPlayerTask(2));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(3));
	EXPECT_NE((void*)0, rTaskUnit.getPlayerTask(3));

	// 删除1，留下2，3
	rTaskUnit.removeTask(1);
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(3));

	// 删除1，2，留下3
	rTaskUnit.removeTask(2);
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(3));

	// 删除1，2，3
	rTaskUnit.removeTask(3);
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(3));
}

// 任务提交条件
TEST(TaskModule, checkCommitCondition)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	// 检查没有对应的任务配置
	EXPECT_EQ(ERROR_TASK_CONFIG_IS_NULL, rTaskUnit.checkCommitTask(2, -1));
	// 检查玩家身上没有对应的任务
	EXPECT_EQ(ERROR_TASK_HAS_NO_TASK, rTaskUnit.checkCommitTask(1, -1));
	rTaskUnit.pushBackPlayerTask(1);
	// 检查玩家身上没有足够的道具
	EXPECT_EQ(ERROR_TASK_ITEM_NUM_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));

	CItemUnit& rItemUnit = tEntityPlayer.getItemUnit();
	int nTaskItemID = ITEM_ID_PILE;
	int nTaskItemNum = 24;
	rItemUnit.insertItem(&nTaskItemID, &nTaskItemNum, 1);
	// 检查玩家身上没有足够的道具
	EXPECT_EQ(ERROR_TASK_ITEM_NUM_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));
	nTaskItemNum = 1;
	rItemUnit.insertItem(&nTaskItemID, &nTaskItemNum, 1);
	// 检查没有刷够怪
	EXPECT_EQ(ERROR_TASK_KILL_OGRE_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));

	rTaskUnit.refreshTask(emComplete_KillOgre, TASK_OGRE_ID, 9);
	// 检查没有刷够怪
	EXPECT_EQ(ERROR_TASK_KILL_OGRE_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));
	rTaskUnit.refreshTask(emComplete_KillOgre, TASK_OGRE_ID, 1);
	// 检查玩家等级不足
	EXPECT_EQ(ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));

	tEntityPlayer.setLevel(9);
	// 检查玩家等级不足
	EXPECT_EQ(ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));
	tEntityPlayer.setLevel(10);
	EXPECT_EQ(SUCCESS, rTaskUnit.checkCommitTask(1, -1));
}