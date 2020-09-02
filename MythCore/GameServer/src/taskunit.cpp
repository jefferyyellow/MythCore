#include "taskunit.h"
#include "taskmodule.hxx.pb.h"
#include "common.hxx.pb.h"
#include "locallogjob.h"
#include "scenejob.h"
#include "errcode.h"
#include "entityplayer.h"
#include "taskmodule.h"

void CTaskUnit::onAcceptTaskRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CAcceptTaskRequest* pAcceptTaskRequest = static_cast<CAcceptTaskRequest*>(pMessage);
	MYTH_ASSERT(NULL != pAcceptTaskRequest, return);

	int nTaskID = pAcceptTaskRequest->taskid();
	//int nNpcEntityID = pAcceptTaskRequest->npcentityid();

	CTaskConfig* pTaskConfig = CTaskModule::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		sendAcceptTaskResponse(ERROR_TASK_CONFIG_IS_NULL, nTaskID);
		return;
	}

	//CEntity* pEntity = static_cast<CEntity*>(CObjPool::Inst()->getObj(nNpcEntityID));
	//if (NULL == pEntity)
	//{
	//	sendAcceptTaskResponse(ERROR_TASK_NPC_IS_VALID, nTaskID);
	//	return;
	//}

	//if (pEntity->getEntityType() != emEntityType_FuncNPC)
	//{
	//	sendAcceptTaskResponse(ERROR_TASK_NPC_IS_VALID, nTaskID);
	//	return;
	//}

	//if (pEntity->getMapID() != mPlayer.getMapID())
	//{
	//	sendAcceptTaskResponse(ERROR_TASK_FAR_FROM_NPC, nTaskID);
	//	return;
	//}

	int nResult = checkAcceptTask(nTaskID);
	if (SUCCESS != nResult)
	{
		sendAcceptTaskResponse(nResult, nTaskID);
		return;
	}

	// 检查任务是否关闭
	nResult = checkCloseTask(nTaskID);
	if (SUCCESS != nResult)
	{
		sendAcceptTaskResponse(nResult, nTaskID);
		return;
	}

	if (checkTaskComplete(nTaskID))
	{
		sendAcceptTaskResponse(ERROR_TASK_ALREADY_END, nTaskID);
		return;
	}

	if (checkPlayerTaskListFull())
	{
		sendAcceptTaskResponse(ERROR_TASK_PLAYER_TASK_LIST_FULL, nTaskID);
		return;
	}
	pushBackPlayerTask(nTaskID);
	sendAcceptTaskResponse(SUCCESS, nTaskID);
	giveAcceptTaskReward(nTaskID);

}

void CTaskUnit::sendAcceptTaskResponse(int nResult, int nTaskID)
{
	CAcceptTaskResponse tAcceptTaskResponse;
	tAcceptTaskResponse.set_result(nResult);
	tAcceptTaskResponse.set_taskid(nTaskID);

	CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_ACCEPT_TASK, &tAcceptTaskResponse);
}

void CTaskUnit::onSubmitTaskRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CSubmitTaskRequest* pSubmitTaskRequest = static_cast<CSubmitTaskRequest*>(pMessage);
	MYTH_ASSERT(NULL != pSubmitTaskRequest, return);

	int nTaskID = pSubmitTaskRequest->taskid();
	int nItemIndex = pSubmitTaskRequest->itemindex();

	int nResult = checkCommitTask(nTaskID, nItemIndex);
	if (SUCCESS != nResult)
	{
		sendSubmitTaskResponse(nResult, 0);
		return;
	}

	nResult = checkTaskReward(nTaskID);
	if (SUCCESS != nResult)
	{
		return;
	}

	if (checkTaskComplete(nTaskID))
	{
		sendAcceptTaskResponse(ERROR_TASK_ALREADY_END, nTaskID);
		return;
	}

	completeTask(nTaskID);
	giveCompleteTaskReward(nTaskID);
	removeTaskItem(nTaskID, nItemIndex);
	sendSubmitTaskResponse(SUCCESS, nTaskID);
}

void CTaskUnit::sendSubmitTaskResponse(int nResult, int nTaskID)
{
	CSubmitTaskResponse tResponse;
	tResponse.set_result(nResult);
	tResponse.set_taskid(nTaskID);

	CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_SUBMIT_TASK, &tResponse);
}


/// 处理放弃任务请求
void CTaskUnit::onAbortTaskRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CAbortTaskRequest* pAbortTaskRequest = static_cast<CAbortTaskRequest*>(pMessage);
	MYTH_ASSERT(NULL != pAbortTaskRequest, return);

	int nTaskID = pAbortTaskRequest->taskid();

	removeTask(nTaskID);
	sendAbortTaskResponse(SUCCESS, nTaskID);

}
/// 发送放弃任务回应
void CTaskUnit::sendAbortTaskResponse(int nResult, int nTaskID)
{
	CAbortTaskResponse tResponse;
	tResponse.set_result(nResult);
	tResponse.set_taskid(nTaskID);
	CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_ABORT_TASK, &tResponse);
}

/// 发送更新任务进度通知
void CTaskUnit::sendUpdateTaskProcessNotify(int nTaskID, int nCondIndex, int nParam)
{
	CUpdateTaskProcessNotify tNotify;
	tNotify.set_taskid(nTaskID);
	tNotify.set_param(nParam);
	CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_NOTIFY_UPDATE_TASK_PROCESS, &tNotify);
}

// 玩家身上是否有这个任务
bool CTaskUnit::checkPlayerHasTask(int nTaskID)
{
	for (unsigned int i = 0; i < mTaskList.size(); ++i)
	{
		if (mTaskList[i].getTaskID() == nTaskID)
		{
			return true;
		}
	}

	return false;
}

// 通过任务ID得到任务
CPlayerTask* CTaskUnit::getPlayerTask(int nTaskID)
{
	for (unsigned int i = 0; i < mTaskList.size(); ++i)
	{
		if (mTaskList[i].getTaskID() == nTaskID)
		{
			return &mTaskList[i];
		}
	}

	return NULL;
}

void CTaskUnit::setFromPB(PBTaskList* pbTaskList)
{
	if (NULL == pbTaskList)
	{
		return;
	}

	for (int i = 0; i < pbTaskList->completetasks_size(); ++ i)
	{
		mCompleteTasks.setBitSet(i, pbTaskList->completetasks(i));
	}

	for (unsigned int i = 0; i < mTaskList.max_size() && i < (unsigned int)pbTaskList->tasklist_size(); ++i)
	{
		CPlayerTask tPlayerTask;
		PBTask* pbTask = pbTaskList->mutable_tasklist(i);
		if (NULL == pbTask)
		{
			continue;
		}

		tPlayerTask.setTaskID(pbTask->taskid());
		for (int j = 0; j < MAX_COMPLETE_CONDITION && j < pbTask->param_size(); ++j)
		{
			tPlayerTask.setTaskParam(j, pbTask->param(j));
		}
		mTaskList.push_back(tPlayerTask);
	}
}

void CTaskUnit::createToPB(PBTaskList* pbTaskList)
{
	if (NULL == pbTaskList)
	{
		return;
	}
	for (int i = 0; i < mCompleteTasks.getBitSetSize(); ++ i)
	{
		pbTaskList->add_completetasks(mCompleteTasks.getBitSet(i));
	}

	for (unsigned int i = 0; i < mTaskList.size(); ++i)
	{
		PBTask* pbTask = pbTaskList->add_tasklist();
		if (NULL == pbTask)
		{
			continue;
		}

		pbTask->set_taskid(mTaskList[i].getTaskID());
		for (int j = 0; j < MAX_COMPLETE_CONDITION; ++j)
		{
			pbTask->add_param(mTaskList[i].getTaskParam(j));
		}
	}
}

// 加入玩家任务
void CTaskUnit::pushBackPlayerTask(int nTaskID)
{
	CPlayerTask tPlayerTask;
	tPlayerTask.setTaskID(nTaskID);
	mTaskList.push_back(tPlayerTask);
}

void CTaskUnit::completeTask(int nTaskID)
{
	removeTask(nTaskID);
	mCompleteTasks.setBit(nTaskID);
}

bool CTaskUnit::checkTaskComplete(int nTaskID)
{
	return mCompleteTasks.getBit(nTaskID);
}

void CTaskUnit::removeTask(int nTaskID)
{
	PLAYER_TASK_LIST::iterator it = mTaskList.begin();
	for (; it != mTaskList.end(); ++ it)
	{
		if (it->getTaskID() == nTaskID)
		{
			mTaskList.erase(it);
			break;
		}
	}
}

// 检查是否可以接受任务
int CTaskUnit::checkAcceptTask(int nTaskID)
{
	CTaskConfig* pTaskConfig = CTaskModule::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		return ERROR_TASK_CONFIG_IS_NULL;
	}
	if (checkTaskComplete(nTaskID))
	{
		return ERROR_TASK_PLAYER_ALREADY_COMPLETE;
	}

	if (checkPlayerHasTask(nTaskID))
	{
		return ERROR_TASK_PLAYER_ALREADY_ACCEPT;
	}

	for (unsigned int i = 0; i < pTaskConfig->mAcceptCondition.size(); ++i)
	{
		CTaskCondition& rCondition = pTaskConfig->mAcceptCondition[i];
		if (emAccept_None == rCondition.mType)
		{
			break;
		}
		int nResult = checkAcceptCondition((EmAcceptCondition)rCondition.mType, rCondition.mParam[0], rCondition.mParam[1]);
		if (SUCCESS != nResult)
		{
			return nResult;
		}
	}

	return SUCCESS;
}

// 检查接受任务条件
int CTaskUnit::checkAcceptCondition(EmAcceptCondition eCondition, int nParam1, int nParam2)
{
	switch (eCondition)
	{
		case emAccept_PreTask:
		{
			// nParam1是前置任务ID
			if (!checkTaskComplete(nParam1))
			{
				return ERROR_TASK_PRE_TASK_NOT_COMPLETE;
			}
			break;
		}
		case emAccept_Level:
		{
			// nParam1是要求的玩家等级
			if (mPlayer.getLevel() < nParam1)
			{
				return ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH;
			}
			break;
		}
		default:
		{
			return ERR_UNKNOWN;
		}
	}

	return SUCCESS;
}


// 检查是否可以关闭任务
int CTaskUnit::checkCloseTask(int nTaskID)
{
	CTaskConfig* pTaskConfig = CTaskModule::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		return ERROR_TASK_CONFIG_IS_NULL;
	}
	for (unsigned int i = 0; i < pTaskConfig->mCloseCondition.size(); ++i)
	{
		CTaskCondition& rCondition = pTaskConfig->mCloseCondition[i];
		if (emClose_None == rCondition.mType)
		{
			break;
		}
		int nResult = checkCloseCondition(rCondition);
		if (SUCCESS != nResult)
		{
			return nResult;
		}
	}

	return SUCCESS;
}
// 检查是否满足关闭任务条件
int CTaskUnit::checkCloseCondition(CTaskCondition& rCondition)
{
	switch (rCondition.mType)
	{
		case emClose_Level:
		{
			// 已经达到指定等级而关闭
			if (mPlayer.getLevel() >= rCondition.mParam[0])
			{
				return ERROR_TASK_CLOSE_LEVEL;
			}
			break;
		}
		case emClose_PreTask:
		{
			// 完成了只能的任务而关闭
			if (checkTaskComplete(rCondition.mParam[0]))
			{
				return ERROR_TASK_CLOSE_PRE_TASK_COMPLETE;
			}
			break;
		}
		default:
			break;
	}
	return SUCCESS;
}

// 检查是否可以提交任务
int CTaskUnit::checkCommitTask(int nTaskID, int nItemIndex)
{
	CTaskConfig* pTaskConfig = CTaskModule::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		return ERROR_TASK_CONFIG_IS_NULL;
	}

	CPlayerTask* pPlayerTask = getPlayerTask(nTaskID);
	if (NULL == pPlayerTask)
	{
		return ERROR_TASK_HAS_NO_TASK;
	}

	for (unsigned int i = 0; i < pTaskConfig->mCompleteCondition.size(); ++i)
	{
		CTaskCondition& rCondition = pTaskConfig->mCompleteCondition[i];
		if (emComplete_None == rCondition.mType)
		{
			continue;
		}
		int nResult = checkCommitCondition(pPlayerTask, i, rCondition, nItemIndex);
		if (SUCCESS != nResult)
		{
			return nResult;
		}
	}

	return SUCCESS;
}

// 检查是否满足提交任务条件
int CTaskUnit::checkCommitCondition(CPlayerTask* pPlayerTask, int nParamIndex, CTaskCondition& rCondition, int nItemIndex)
{
	if (NULL == pPlayerTask)
	{
		return ERR_PARAMETER_INVALID;
	}

	switch (rCondition.mType)
	{
		case emComplete_KillOgre:
		{
			if (pPlayerTask->getTaskParam(nParamIndex) < rCondition.mParam[1])
			{
				return ERROR_TASK_KILL_OGRE_NOT_ENOUGH;
			}
			break;
		}
		case emComplete_HoldItem:
		{
			if (nItemIndex < 0)
			{
				int nItemNum = mPlayer.getItemUnit().getBag().hasItem(rCondition.mParam[0]);
				if (nItemNum < rCondition.mParam[1])
				{
					return ERROR_TASK_ITEM_NUM_NOT_ENOUGH;
				}
			}
			else
			{
				CItemObject* pItemObject = static_cast<CItemObject*>(mPlayer.getItemUnit().getBag().getItem(nItemIndex));
				if (NULL == pItemObject)
				{
					return ERROR_TASK_ITEM_DATA_IS_NULL;
				}

				if (pItemObject->GetItemID() == rCondition.mParam[0])
				{
					return ERROR_TASK_ITEM_INDEX_NOT_MATCH;
				}

				if ((int)pItemObject->GetItemNum() < rCondition.mParam[1])
				{
					return ERROR_TASK_ITEM_NUM_NOT_ENOUGH;
				}
			}

			break;
		}
		case emComplete_Level:
		{
			if (mPlayer.getLevel() < rCondition.mParam[0])
			{
				return ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH;
			}
			break;
		}
		case emComplete_PassFB:
		{
			if (pPlayerTask->getTaskParam(nParamIndex) < rCondition.mParam[3])
			{
				return ERROR_TASK_INSTANCE_TIMES_NOT_ENOUGH;
			}
			break;
		}
		case emComplete_Collect:
		{
			if (pPlayerTask->getTaskParam(nParamIndex) < rCondition.mParam[1])
			{
				return ERROR_TASK_COLLECT_NUM_NOT_MATCH;
			}
			break;
		}
		case emComplete_Talk:
		{
			break;
		}
		default:
		{
			return ERR_UNKNOWN;
		}
	}

	return SUCCESS;
}

// 扣除任务道具
void CTaskUnit::removeTaskItem(int nTaskID, int nItemIndex)
{
	CTaskConfig* pTaskConfig = CTaskModule::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		return;
	}

	for (unsigned int i = 0; i < pTaskConfig->mCompleteCondition.size(); ++i)
	{
		CTaskCondition& rCondition = pTaskConfig->mCompleteCondition[i];
		if (emComplete_None == rCondition.mType)
		{
			continue;
		}
		if (emComplete_HoldItem == rCondition.mType)
		{
			if (nItemIndex < 0)
			{
				mPlayer.getItemUnit().removeItemByID(rCondition.mParam[0], rCondition.mParam[1]);
			}
			else
			{
				mPlayer.getItemUnit().removeItem(nItemIndex, rCondition.mParam[1]);
			}
			break;
		}
	}
}

// 刷新任务条件
void CTaskUnit::refreshTask(EmCompleteCondition eCondition, int nParam0, int nParam1, int nParam2, int nParam3)
{
	for (unsigned int i = 0; i < mTaskList.size(); ++i)
	{
		CPlayerTask& rPlayerTask = mTaskList[i];
		CTaskConfig* pTaskConfig = CTaskModule::Inst()->getTaskConfig(rPlayerTask.getTaskID());
		if (NULL == pTaskConfig)
		{
			continue;
		}

		for (unsigned int nConditionNum = 0; nConditionNum < pTaskConfig->mCompleteCondition.size(); ++nConditionNum)
		{
			if (pTaskConfig->mCompleteCondition[nConditionNum].mType == emComplete_None)
			{
				break;
			}
			
			if (eCondition != pTaskConfig->mCompleteCondition[nConditionNum].mType)
			{
				continue;
			}

			switch (pTaskConfig->mCompleteCondition[nConditionNum].mType)
			{
				case emComplete_KillOgre:
				case emComplete_Collect:
				{
					if (pTaskConfig->mCompleteCondition[nConditionNum].mParam[0] == nParam0)
					{
						rPlayerTask.setTaskParam(nConditionNum, rPlayerTask.getTaskParam(nConditionNum) + nParam1);
						sendUpdateTaskProcessNotify(rPlayerTask.getTaskID(), eCondition, rPlayerTask.getTaskParam(nConditionNum));
					}
					break;
				}
				case emComplete_PassFB:
				{
					CTaskCondition& rCondition = pTaskConfig->mCompleteCondition[nConditionNum];
					if (rCondition.mParam[0] == nParam0 && rCondition.mParam[1] == nParam1 && rCondition.mParam[2] == nParam2)
					{
						rPlayerTask.setTaskParam(nConditionNum, rPlayerTask.getTaskParam(nConditionNum) + 1);
						sendUpdateTaskProcessNotify(rPlayerTask.getTaskID(), eCondition, rPlayerTask.getTaskParam(nConditionNum));
					}
					break;
				}
				default:
					break;
			}

			break;
		}
	}
}

// 给完成任务奖励
void CTaskUnit::giveCompleteTaskReward(int nTaskID)
{
	CTaskConfig* pTaskConfig = CTaskModule::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		return;
	}
	giveTaskReward(pTaskConfig->mPrizeList);
}

// 给接受任务奖励
void CTaskUnit::giveAcceptTaskReward(int nTaskID)
{
	CTaskConfig* pTaskConfig = CTaskModule::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		return;
	}
	giveTaskReward(pTaskConfig->mAcceptPrizeList);
}

// 给任务奖励
void CTaskUnit::giveTaskReward(CTaskConfig::TASK_PRIZE_LIST& rPrizeList)
{
	for (unsigned int i = 0; i < rPrizeList.size(); ++i)
	{
		CTaskReward& rReward = rPrizeList[i];
		switch (rReward.mType)
		{
			case emTaskPrize_Item:
			{
				mPlayer.getItemUnit().insertAllItem(&rReward.mParam[0], &rReward.mParam[1], 1);
				break;
			}
			default:
				break;
		}
	}
}

// 检查是否可以得到奖励
int CTaskUnit::checkTaskReward(int nTaskID)
{
	CTaskConfig* pTaskConfig =CTaskModule::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		return ERR_PARAMETER_INVALID;
	}

	int nItemID[MAX_TASK_PRIZE_NUM] = { 0 };
	int nItemNum[MAX_TASK_PRIZE_NUM] = { 0 };
	int nItemCount = 0;
	for (unsigned int i = 0; i < pTaskConfig->mPrizeList.size() && i < MAX_TASK_PRIZE_NUM; ++i)
	{
		if (emTaskPrize_Item != pTaskConfig->mPrizeList[i].mType)
		{
			continue;
		}
		nItemID[nItemCount] = pTaskConfig->mPrizeList[i].mParam[0];
		nItemNum[nItemCount] = pTaskConfig->mPrizeList[i].mParam[1];
		++nItemCount;
	}

	bool bResult = mPlayer.getItemUnit().checkItemSpace(nItemID, nItemNum, nItemCount);
	if (!bResult)
	{
		return ERR_BAGGAGE_IS_FULL;
	}

	return SUCCESS;
}
