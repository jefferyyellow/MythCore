#include "taskmodule.h"
#include "taskmodule.hxx.pb.h"
#include "entityplayer.h"
#include "fileutility.h"

void CTaskModule::clear()
{
	for (int i = 0; i < MAX_TASK_ID; ++ i)
	{
		if (NULL != mTaskConfig[i])
		{
			delete mTaskConfig[i];
			mTaskConfig[i] = NULL;
		}
	}
}

/// 启动服务器
void CTaskModule::onLaunchServer()
{
	loadAllTaskConfig("gameserverconfig/tasks/*.xml");
}

/// 启动完成检查
bool CTaskModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CTaskModule::onLaunchComplete()
{

}

/// 退出服务器
void CTaskModule::onExitServer()
{

}

/// 新一天到来
void CTaskModule::onNewDayCome()
{

}

/// 新一周到来
void CTaskModule::onNewWeekCome()
{

}

/// 建立实体
void CTaskModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
void CTaskModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CTaskModule::onTimer(unsigned int nTickOffset)
{

}

void CTaskModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_ACCEPT_TASK:
		{
			pPlayer->getTaskUnit().onAcceptTaskRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_SUBMIT_TASK:
		{
			pPlayer->getTaskUnit().onSubmitTaskRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_ABORT_TASK:
		{
			pPlayer->getTaskUnit().onAbortTaskRequest(pMessage);
			break;
		}
		default:
			break;
	}
}


// 加载配置
int CTaskModule::loadAllTaskConfig(const char* pPath)
{
	MYTH_ASSERT(pPath == NULL, return -1);
	CDir tDir;
	char tPathFile[STR_LENGTH_256] = {0};
	char tFileName[STR_LENGTH_256] = {0};
	tDir.findFirstFile(pPath, tFileName, sizeof(tFileName));
	while (tFileName[0] != '\0')
	{
		snprintf(tPathFile, sizeof(tPathFile) - 1, "%s%s", "gameserverconfig/tasks/", tFileName);
		loadTaskConfig(tPathFile);
		tDir.nextFile(tFileName, sizeof(tFileName));
	}

	return 0;
}

// 加载单个任务配置
int CTaskModule::loadTaskConfig(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		return -1;
	}
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pFilePath))
	{
		LOG_ERROR("load task config failure, %d", pFilePath);
		return -1;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		LOG_ERROR("task config file has no root element, %s", pFilePath);
		return -1;
	}

	XMLElement* pIdElem = pRoot->FirstChildElement("TaskId");
	if (NULL == pIdElem)
	{
		LOG_ERROR("task config file has no id element, %s", pFilePath);
		return -1;
	}

	int nTaskID = pIdElem->IntAttribute("Value");
	if (nTaskID < 0 || nTaskID >= MAX_TASK_ID)
	{
		LOG_ERROR("task id invalid, %s, task id: %d", pFilePath, nTaskID);
		return -1;
	}

	CTaskConfig* pTaskConfig = new CTaskConfig;
	if (NULL == pTaskConfig)
	{
		LOG_ERROR("allocate CTaskConfig failure, %s", pFilePath);
		return -1;
	}
	pTaskConfig->mId = nTaskID;

	XMLElement* pTypeElem = pRoot->FirstChildElement("TaskType");
	if (NULL != pTypeElem)
	{
		pTaskConfig->mType = (EmTaskType)(pTypeElem->IntAttribute("Value"));
	}

	XMLElement* pAcceptMapElem = pRoot->FirstChildElement("AcceptMap");
	if (NULL != pAcceptMapElem)
	{
		pTaskConfig->mAcceptMapId = pAcceptMapElem->IntAttribute("Value");
	}

	XMLElement* pAcceptNpcElem = pRoot->FirstChildElement("AcceptNpc");
	if (NULL != pAcceptNpcElem)
	{
		pTaskConfig->mAcceptNpc = pAcceptNpcElem->IntAttribute("Value");
	}

	XMLElement* pSubmitMapElem = pRoot->FirstChildElement("SubmitMap");
	if (NULL != pSubmitMapElem)
	{
		pTaskConfig->mSubmitMapId = pSubmitMapElem->IntAttribute("Value");
	}

	XMLElement* pSubmitNpcElem = pRoot->FirstChildElement("SubmitNpc");
	if (NULL != pSubmitNpcElem)
	{
		pTaskConfig->mSubmitNpc = pSubmitNpcElem->IntAttribute("Value");
	}

	XMLElement* pAccpetCondElem = pRoot->FirstChildElement("AccpetCond");
	if (NULL != pAccpetCondElem)
	{
		XMLElement* pCondElem = pAccpetCondElem->FirstChildElement("Cond");
		for (; NULL != pCondElem; pCondElem = pCondElem->NextSiblingElement("Cond"))
		{
			CTaskCondition tCondition;
			loadTaskCond(pCondElem, tCondition);
			pTaskConfig->mAcceptCondition.push_back(tCondition);
		}
	}

	XMLElement* pCompleteCondElem = pRoot->FirstChildElement("CompleteCond");
	if (NULL != pCompleteCondElem)
	{
		XMLElement* pCondElem = pCompleteCondElem->FirstChildElement("Cond");
		for (; NULL != pCondElem; pCondElem = pCondElem->NextSiblingElement("Cond"))
		{
			CTaskCondition tCondition;
			loadTaskCond(pCondElem, tCondition);
			pTaskConfig->mCompleteCondition.push_back(tCondition);
		}
	}

	XMLElement* pTaskAwardElem = pRoot->FirstChildElement("TaskAward");
	if (NULL != pTaskAwardElem)
	{
		XMLElement* pCondElem = pTaskAwardElem->FirstChildElement("Cond");
		for (; NULL != pCondElem; pCondElem = pCondElem->NextSiblingElement("Cond"))
		{
			CTaskReward tTaskReward;
			tTaskReward.mType = pCondElem->IntAttribute("Type");
			tTaskReward.mParam[0] = pCondElem->IntAttribute("Para0");
			tTaskReward.mParam[1] = pCondElem->IntAttribute("Para1");
			tTaskReward.mParam[2] = pCondElem->IntAttribute("Para2");
			tTaskReward.mParam[3] = pCondElem->IntAttribute("Para3");
			pTaskConfig->mPrizeList.push_back(tTaskReward);
		}
	}
	mTaskConfig[nTaskID] = pTaskConfig;
	return 0;
}

// 加载条件
void CTaskModule::loadTaskCond(XMLElement* pXmlElem, CTaskCondition& rCondition)
{
	if (NULL == pXmlElem)
	{
		return;
	}

	rCondition.mType = pXmlElem->IntAttribute("Type");
	rCondition.mParam[0] = pXmlElem->IntAttribute("Para0");
	rCondition.mParam[1] = pXmlElem->IntAttribute("Para1");
	rCondition.mParam[2] = pXmlElem->IntAttribute("Para2");
	rCondition.mParam[3] = pXmlElem->IntAttribute("Para3");
}