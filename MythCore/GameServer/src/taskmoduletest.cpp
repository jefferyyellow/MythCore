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

	// ����������ǰ������
	CTaskCondition tAccpetCond;
	tAccpetCond.mType = 1;
	tAccpetCond.mParam[0] =1999;
	pNewConfig->mAcceptCondition.push_back(tAccpetCond);

	// �����������ȼ�
	CTaskCondition tAccpetCond2;
	tAccpetCond2.mType = 2;
	tAccpetCond2.mParam[0] = 10;
	tAccpetCond2.mParam[1] = 10;
	pNewConfig->mAcceptCondition.push_back(tAccpetCond2);

	// ���������ӵ�е���
	CTaskCondition tCompleteCond;
	tCompleteCond.mType = 2;
	tCompleteCond.mParam[0] = ITEM_ID_PILE;
	tCompleteCond.mParam[1] = 25;
	pNewConfig->mCompleteCondition.push_back(tCompleteCond);

	// ���������ɱ�ּ���
	CTaskCondition tCompleteCond2;
	tCompleteCond2.mType = 1;
	tCompleteCond2.mParam[0] = TASK_OGRE_ID;
	tCompleteCond2.mParam[1] = 10;
	pNewConfig->mCompleteCondition.push_back(tCompleteCond2);

	// ����������ȼ�
	CTaskCondition tCompleteCond3;
	tCompleteCond3.mType = 3;
	tCompleteCond3.mParam[0] = 10;
	pNewConfig->mCompleteCondition.push_back(tCompleteCond3);

	// ��ɽ���������
	CTaskReward tReward;
	tReward.mType = 1;
	tReward.mParam[0] = 11015;
	tReward.mParam[1] = 20;
	pNewConfig->mPrizeList.push_back(tReward);

	// ��ɽ���������
	CTaskReward tReward2;
	tReward2.mType = 1;
	tReward2.mParam[0] = 10001;
	tReward2.mParam[1] = 2;
	pNewConfig->mPrizeList.push_back(tReward2);
	
	CTaskModule::Inst()->setTaskConfig(pNewConfig->mId, pNewConfig);
}

// ��������Ƿ����
TEST(TaskModule, checkTaskComplete)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	rTaskUnit.setTaskComplete(1);
	EXPECT_FALSE(rTaskUnit.checkTaskComplete(0));
	EXPECT_TRUE(rTaskUnit.checkTaskComplete(1));
	EXPECT_FALSE(rTaskUnit.checkTaskComplete(2));
}

// ��������������
TEST(TaskModule, checkAcceptCondition)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	// ���ǰ���������
	tEntityPlayer.setLevel(9);
	EXPECT_EQ(ERROR_TASK_PRE_TASK_NOT_COMPLETE, rTaskUnit.checkAcceptTask(1));
	tEntityPlayer.setLevel(10);
	EXPECT_EQ(ERROR_TASK_PRE_TASK_NOT_COMPLETE, rTaskUnit.checkAcceptTask(1));

	// �������ȼ�����
	rTaskUnit.setTaskComplete(1999);
	tEntityPlayer.setLevel(9);
	EXPECT_EQ(ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH, rTaskUnit.checkAcceptTask(1));
	tEntityPlayer.setLevel(10);
	EXPECT_EQ(0, rTaskUnit.checkAcceptTask(1));
	tEntityPlayer.setLevel(11);
	EXPECT_EQ(0, rTaskUnit.checkAcceptTask(1));
}

// ����������
TEST(TaskModule, checkAcceptTask)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	// �Ѿ�������1
	rTaskUnit.pushBackPlayerTask(1);
	EXPECT_EQ(ERROR_TASK_PLAYER_ALREADY_ACCEPT, rTaskUnit.checkAcceptTask(1));

	rTaskUnit.removeTask(1);
	// �Ѿ������1
	rTaskUnit.setTaskComplete(1);
	EXPECT_EQ(ERROR_TASK_PLAYER_ALREADY_COMPLETE, rTaskUnit.checkAcceptTask(1));

	// û�ж�Ӧ������
	EXPECT_EQ(ERROR_TASK_CONFIG_IS_NULL, rTaskUnit.checkAcceptTask(2));
}

// ���������ϵ����������ɾ����
TEST(TaskModule, pushBackPlayerTask)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	
	// ֻ������1��û��2��3
	rTaskUnit.pushBackPlayerTask(1);
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_NE((void*)0, rTaskUnit.getPlayerTask(1));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_EQ((void*)0, rTaskUnit.getPlayerTask(2));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(3));
	EXPECT_EQ((void*)0, rTaskUnit.getPlayerTask(3));

	// ֻ������1��2��û��3
	rTaskUnit.pushBackPlayerTask(2);
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(3));
	
	// ������1��2��3
	rTaskUnit.pushBackPlayerTask(3);
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_NE((void*)0, rTaskUnit.getPlayerTask(1));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_NE((void*)0, rTaskUnit.getPlayerTask(2));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(3));
	EXPECT_NE((void*)0, rTaskUnit.getPlayerTask(3));

	// ɾ��1������2��3
	rTaskUnit.removeTask(1);
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(3));

	// ɾ��1��2������3
	rTaskUnit.removeTask(2);
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_TRUE(rTaskUnit.checkPlayerHasTask(3));

	// ɾ��1��2��3
	rTaskUnit.removeTask(3);
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(1));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(2));
	EXPECT_FALSE(rTaskUnit.checkPlayerHasTask(3));
}

// �����ύ����
TEST(TaskModule, checkCommitCondition)
{
	CEntityPlayer tEntityPlayer;
	CTaskUnit& rTaskUnit = tEntityPlayer.getTaskUnit();
	// ���û�ж�Ӧ����������
	EXPECT_EQ(ERROR_TASK_CONFIG_IS_NULL, rTaskUnit.checkCommitTask(2, -1));
	// ����������û�ж�Ӧ������
	EXPECT_EQ(ERROR_TASK_HAS_NO_TASK, rTaskUnit.checkCommitTask(1, -1));
	rTaskUnit.pushBackPlayerTask(1);
	// ����������û���㹻�ĵ���
	EXPECT_EQ(ERROR_TASK_ITEM_NUM_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));

	CItemUnit& rItemUnit = tEntityPlayer.getItemUnit();
	int nTaskItemID = ITEM_ID_PILE;
	int nTaskItemNum = 24;
	rItemUnit.insertItem(&nTaskItemID, &nTaskItemNum, 1);
	// ����������û���㹻�ĵ���
	EXPECT_EQ(ERROR_TASK_ITEM_NUM_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));
	nTaskItemNum = 1;
	rItemUnit.insertItem(&nTaskItemID, &nTaskItemNum, 1);
	// ���û��ˢ����
	EXPECT_EQ(ERROR_TASK_KILL_OGRE_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));

	rTaskUnit.refreshTask(emComplete_KillOgre, TASK_OGRE_ID, 9);
	// ���û��ˢ����
	EXPECT_EQ(ERROR_TASK_KILL_OGRE_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));
	rTaskUnit.refreshTask(emComplete_KillOgre, TASK_OGRE_ID, 1);
	// �����ҵȼ�����
	EXPECT_EQ(ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));

	tEntityPlayer.setLevel(9);
	// �����ҵȼ�����
	EXPECT_EQ(ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH, rTaskUnit.checkCommitTask(1, -1));
	tEntityPlayer.setLevel(10);
	EXPECT_EQ(SUCCESS, rTaskUnit.checkCommitTask(1, -1));
}