#include "taskunit.h"
#include "taskmodule.hxx.pb.h"
#include "locallogjob.h"
void CTaskUnit::onAcceptTaskRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL == pMessage, return);
	CAcceptTaskRequest* pAcceptTaskRequest = reinterpret_cast<CAcceptTaskRequest*>pMessage;
	MYTH_ASSERT(NULL == pAcceptTaskRequest, return);

	int nTaskID = pAcceptTaskRequest->taskid();


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