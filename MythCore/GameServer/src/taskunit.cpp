#include "taskunit.h"
#include "taskmodule.hxx.pb.h"
#include "locallogjob.h"
#include "scenejob.h"
#include "errcode.h"
#include "entityplayer.h"
void CTaskUnit::onAcceptTaskRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL == pMessage, return);
	CAcceptTaskRequest* pAcceptTaskRequest = reinterpret_cast<CAcceptTaskRequest*>(pMessage);
	MYTH_ASSERT(NULL == pAcceptTaskRequest, return);

	int nTaskID = pAcceptTaskRequest->taskid();
	
	if (mTaskList.size() >= MAX_PLAYER_TASK_NUM)
	{
		sendAcceptTaskResponse(ERR_TASK_TASK_LIST_FULL, 0);
		return;
	}

	int nResult = checkAcceptTask(nTaskID);
	if (SUCCESS != nResult)
	{
		sendAcceptTaskResponse(nResult, 0);
		return;
	}

	
	CPlayerTask tPlayerTask;
	tPlayerTask.init();
	tPlayerTask.setTaskID(nTaskID);
	mTaskList.push_back(tPlayerTask);

	sendAcceptTaskResponse(SUCCESS, nTaskID);
}

void CTaskUnit::sendAcceptTaskResponse(int nResult, int nTaskID)
{
	CAcceptTaskResponse tAcceptTaskResponse;
	tAcceptTaskResponse.set_result(nResult);
	tAcceptTaskResponse.set_taskid(nTaskID);

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_ACCEPT_TASK, &tAcceptTaskResponse);
}

void CTaskUnit::onSubmitTaskRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL == pMessage, return);
	CSubmitTaskRequest* pSubmitTaskRequest = reinterpret_cast<CSubmitTaskRequest*>(pMessage);
	MYTH_ASSERT(NULL == pSubmitTaskRequest, return);

	int nTaskID = pSubmitTaskRequest->taskid();

	short bRepeated = true;
	checkSubmitTask(nTaskID, bRepeated);
	if (bRepeated)
	{
		mCompleteTasks.setBit(nTaskID);
	}
}

void CTaskUnit::sendSubmitTaskResponse(int nResult, int nTaskID)
{
	CSubmitTaskResponse tSubmitTaskResponse;
	tSubmitTaskResponse.set_result(nResult);
	tSubmitTaskResponse.set_taskid(nTaskID);

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_SUBMIT_TASK, &tSubmitTaskResponse);
}

// 检查任务是否能接受
int	CTaskUnit::checkAcceptTask(int nTaskID)
{
	CTaskConfig* pTaskConfig = CTaskConfigManager::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		return ERR_TASK_CONFIG_IS_NULL;
	}

	// 不能重复做的任务需要检查是否已经做过了
	if (!pTaskConfig->mRepeated)
	{
		// 如果已经完成了
		if (mCompleteTasks.getBit(nTaskID))
		{
			return ERR_TASK_ALREADY_COMPLETE;
		}
	}

	for (unsigned int i = 0; i < pTaskConfig->mAcceptCondition.size(); ++ i)
	{
		if (emAccept_None == pTaskConfig->mAcceptCondition[i].mType)
		{
			break;
		}
		switch(pTaskConfig->mAcceptCondition[i].mType)
		{
			case emAccept_PreTask:
			{
				int nPreTaskID = pTaskConfig->mAcceptCondition[i].mParam[0];
				if (!mCompleteTasks.getBit(nPreTaskID))
				{
					return ERR_TASK_PRE_TASK_NOT_COMPLETE;
				}
				break;
			}
			case emAccept_Level:
			{
				int nMinLevel = pTaskConfig->mAcceptCondition[i].mParam[0];
				int nMaxLevel = pTaskConfig->mAcceptCondition[i].mParam[1];
				if (mPlayer.getLevel() < nMinLevel || mPlayer.getLevel() > nMaxLevel)
				{
					return ERR_TASK_PLAYER_LEVEL_INVALID;
				}
				break;
			}
		}

	}

	return SUCCESS;
}

/// 检查任务是否能提交
int CTaskUnit::checkSubmitTask(int nTaskID, short& bRepeated)
{
	CTaskConfig* pTaskConfig = CTaskConfigManager::Inst()->getTaskConfig(nTaskID);
	if (NULL == pTaskConfig)
	{
		return ERR_TASK_CONFIG_IS_NULL;
	}

	CPlayerTask* pPlayerTask = getPlayerTask(nTaskID);
	if (NULL == pPlayerTask)
	{
		return ERR_TASK_PLAYER_HAS_NO_TASK;
	}

	// 不能重复做的任务需要检查是否已经做过了
	if (!pTaskConfig->mRepeated)
	{
		// 如果已经完成了
		if (mCompleteTasks.getBit(nTaskID))
		{
			return ERR_TASK_ALREADY_COMPLETE;
		}
	}

	for (unsigned int i = 0; i < pTaskConfig->mCompleteCondition.size(); ++ i)
	{
		if (emComplete_None == pTaskConfig->mCompleteCondition[i].mType)
		{
			break;
		}
		switch(pTaskConfig->mCompleteCondition[i].mType)
		{
			case emComplete_KillOgre:
			{
				if (pPlayerTask->getTaskParam(i) < pTaskConfig->mCompleteCondition[i].mParam[1])
				{
					return ERR_TASK_KILL_OGRE_NOT_ENOUGH;
				}
				break;
			}
			case emComplete_HoldItem:
			{
				if (mPlayer.getItemUnit().hasItem(pTaskConfig->mCompleteCondition[i].mParam[0]) < pTaskConfig->mCompleteCondition[i].mParam[1])
				{
					return ERR_TASK_ITEM_NUM_NOT_ENOUGH;
				}
				break;
			}
		}
	}

	bRepeated = pTaskConfig->mRepeated;
	return SUCCESS;
}

/// 根据ID得到玩家身上的任务
CPlayerTask* CTaskUnit::getPlayerTask(int nTaskID)
{
	for (unsigned int i = 0; i < mTaskList.size(); ++ i)
	{
		if (mTaskList[i].getTaskID() == nTaskID)
		{
			return &mTaskList[i];
		}
	}

	return NULL;
}

/// 删除玩家身上的任务
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

void CTaskUnit::fireEvent(EmCompleteCondition eCondition, int nParam1, int nParam2)
{
	for (unsigned int i = 0; i < mTaskList.size(); ++ i)
	{
		CTaskConfig* pConfig = CTaskConfigManager::Inst()->getTaskConfig(mTaskList[i].getTaskID());
		if (NULL == pConfig)
		{
			continue;
		}

		for (int j = 0; j < MAX_COMPLETE_CONDITION_PARAM; ++ j)
		{
			if (emComplete_None == pConfig->mCompleteCondition[j].mType)
			{
				break;
			}

			if (pConfig->mCompleteCondition[j].mType != eCondition)
			{
				continue;
			}

			if (pConfig->mCompleteCondition[j].mParam[0] != nParam1)
			{
				continue;
			}

			mTaskList[i].setTaskParam(j, mTaskList[i].getTaskParam(j) + nParam2);
			break;
		}
	}
}