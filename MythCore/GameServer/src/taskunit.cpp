#include "taskunit.h"
#include "taskmodule.hxx.pb.h"
#include "locallogjob.h"
#include "scenejob.h"
#include "errcode.h"
void CTaskUnit::onAcceptTaskRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL == pMessage, return);
	CAcceptTaskRequest* pAcceptTaskRequest = reinterpret_cast<CAcceptTaskRequest*>(pMessage);
	MYTH_ASSERT(NULL == pAcceptTaskRequest, return);

	int nTaskID = pAcceptTaskRequest->taskid();


	int nFreeIndex = getFreeTask();
	if (nFreeIndex < 0)
	{
		return;
	}
	mTaskList[nFreeIndex].init();
	mTaskList[nFreeIndex].setTaskID(nTaskID);
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
			return;
		}
	}
}

// 得到一个空的任务
int	CTaskUnit::getFreeTask()
{
	for (int i = 0; i < MAX_PLAYER_TASK_NUM; ++ i)
	{
		if (0 == mTaskList[i].getTaskID())
		{
			return i;
		}
	}

	return -1;
}

void CTaskUnit::fireEvent(EmTaskCondType eTaskCondType, int nParam)
{
	for (int i = 0; i < mTaskListNum; ++ i)
	{
		if (mTaskList[i].getTaskCondType() != eTaskCondType)
		{
			continue;
		}

		if (mTaskList[i].getTaskProgress() != emTaskState_Accept)
		{
			break;
		}

		switch (eTaskCondType)
		{
			case emTaskCondType_KillOgre:
			{
				break;
			}
			case emTaskCondType_ObtainItem:
			{
				break;
			}
		}
	}
}