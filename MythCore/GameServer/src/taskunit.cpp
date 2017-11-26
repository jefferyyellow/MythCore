#include "taskunit.h"
void CTaskUnit::refreshTaskProgress(EmTaskCondType eTaskCondType, int nParam1, int nParam2)
{
	for (int i = 0; i < mTaskListNum; ++ i)
	{
		if (mTaskList[i].getTaskCondType() != eTaskCondType)
		{
			continue;
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