#include "serveractmodule.h"
#include "serveractivity.h"
#include "locallogjob.h"
#include "serverutility.h"
#include "serveractivityunit.h"
#include "entityplayer.h"
#include "scenejob.h"
#include "serveractmodule.hxx.pb.h"
#include "timemanager.h"
CServerActModule::CServerActModule()
{
	init();
	mServerOpenTime = unixTimeStamp("2020-8-4 00:00:00");
}


CServerActModule::~CServerActModule()
{
	clearServerActivity();
}

void CServerActModule::clearServerActivity()
{
	for (int i = 0; i < MAX_SERVER_ACT_NUM; ++i)
	{
		if (NULL != mServerActivity[i])
		{
			delete mServerActivity[i];
		}
		mServerActivity[i] = NULL;
		mAvailActivity[i] = NULL;
	}
	mAvailActivityNum = 0;
}

/// 启动服务器
void CServerActModule::onLaunchServer()
{
	loadServerActivityConfig("gameserverconfig/server_activity/server_activity.xml");
}

/// 启动完成检查
bool CServerActModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CServerActModule::onLaunchComplete()
{
	mServerStartTime = CTimeManager::Inst()->getCurrTime();
}

/// 退出服务器
void CServerActModule::onExitServer()
{

}

/// 新一天到来
void CServerActModule::onNewDayCome()
{
	// 结算结束的活动
	clearEndedActivity();
	// 加载新的一天的活动
	loadServerActivityConfig("gameserverconfig/server_activity/server_activity.xml");
}

/// 新一周到来
void CServerActModule::onNewWeekCome()
{

}

/// 建立实体
void CServerActModule::onCreatePlayer(CEntityPlayer* pPlayer)
{
	
}

/// 销毁实体
void CServerActModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}


}

void CServerActModule::onTimer(unsigned int nTickOffset)
{

}

void CServerActModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_GET_SERVER_ACT:
		{
			break;
		}
	}
}

void CServerActModule::onGetServerActRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pPlayer && NULL != pMessage, return);
	sendGetServerActResponse(pPlayer);
}

void CServerActModule::sendGetServerActResponse(CEntityPlayer* pPlayer)
{
	MYTH_ASSERT(NULL != pPlayer, return);

}

void CServerActModule::loadServerActivityConfig(const char* pConfigFile)
{
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pConfigFile))
	{
		LOG_ERROR("Load server activity config file failure!");
		return;
	}

	XMLElement* pRootElem = tDocument.RootElement();
	if (NULL == pRootElem)
	{
		LOG_ERROR("Don't find server activity config root element");
		return;
	}

	// 清空
	clearServerActivity();

	time_t nStartTime = 0;
	time_t nEndTime = 0;
	time_t nPrizeTime = 0;
	time_t nTimeNow = CTimeManager::Inst()->getCurrTime();

	bool bAlreadyLoad[emServerActTypeMax] = {false};
	char acPathFile[emServerActTypeMax][STR_LENGTH_256] = { 0 };
	XMLElement* pActivityElem = pRootElem->FirstChildElement("Activity");
	for (; NULL != pActivityElem; pActivityElem = pActivityElem->NextSiblingElement("Activity"))
	{
		int nStartDay = pActivityElem->IntAttribute("StartDay");
		int nDurationDay = pActivityElem->IntAttribute("DurationDay");
		if (nStartDay > 0 && nDurationDay > 0)
		{
			nStartTime = mServerOpenTime + (nStartDay - 1) * SECONDS_PER_DAY;
			nEndTime = nStartTime + nDurationDay * SECONDS_PER_DAY - 1;
		}
		else
		{
			nStartTime = unixTimeStamp(pActivityElem->Attribute("StartTime"));
			nEndTime = unixTimeStamp(pActivityElem->Attribute("EndTime"));
		}

		int tTmpTime = pActivityElem->IntAttribute("PrizeTime");
		nPrizeTime = 0;
		if (tTmpTime > 0)
		{
			nPrizeTime = nEndTime + tTmpTime * SECONDS_PER_DAY;
		}

		int nType = pActivityElem->IntAttribute("Type");
		CServerActivity* pActivity = createServerActivity((EmSvrActType)nType);
		if (NULL == pActivity)
		{
			LOG_ERROR("Create server activity failure, %d", nType);
			continue;
		}

		int nActivityID = pActivityElem->IntAttribute("ID");
		if (nActivityID <= 0 || nActivityID >= MAX_SERVER_ACT_NUM)
		{
			LOG_ERROR("Activity ID is invalid, %d", nActivityID);
			continue;
		}

		if (NULL != mServerActivity[nActivityID] )
		{
			LOG_ERROR("Activitys have same activity id, %d", nActivityID);
			continue;
		}
		mServerActivity[nActivityID] = pActivity;

		pActivity->setType(nType);
		pActivity->setSubType(pActivityElem->IntAttribute("SubType"));
		pActivity->setID(nActivityID);
		pActivity->setStartTime(nStartTime);
		pActivity->setEndTime(nEndTime);
		pActivity->setPrizeTime(nPrizeTime);

		// 如果不在活动期间类不处理对应的配置文件，只加载概要信息
		// 如果当前时间比开始时间还早，或者当前时间比结束时间还大
		if (nTimeNow < nStartTime || (nTimeNow >= (nPrizeTime > nEndTime ? nPrizeTime : nEndTime)))
		{
			pActivity->setAvail(false);
			continue;
		}
		pActivity->setAvail(true);

		// 所以
		mAvailActivity[mAvailActivityNum] = pActivity;
		++ mAvailActivityNum;

		if (!bAlreadyLoad[nType])
		{
			bAlreadyLoad[nType] = true;
			snprintf(acPathFile[nType],  STR_LENGTH_256 - 1, "%s%s", "gameserverconfig/server_activity/", pActivity->getConfigFileName());
		}
	}
	for (int nType = 0; nType < emServerActTypeMax; ++ nType)
	{
		if (bAlreadyLoad[nType])
		{
			loadSpecifyActivityConfig(acPathFile[nType]);
		}
	}
}

/// 加载阶段活动配置
void CServerActModule::loadSpecifyActivityConfig(const char* pConfigFile)
{
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pConfigFile))
	{
		LOG_ERROR("Load special server activity config file failure, %s", pConfigFile);
		return;
	}

	XMLElement* pRootElem = tDocument.RootElement();
	if (NULL == pRootElem)
	{
		LOG_ERROR("Don't find special server activity config root element, %s", pConfigFile);
		return;
	}

	XMLElement* pActivityElem = pRootElem->FirstChildElement("Activity");
	for (; NULL != pActivityElem; pActivityElem = pActivityElem->NextSiblingElement("Activity"))
	{
		int nID = pActivityElem->IntAttribute("ID");
		int nType = pActivityElem->IntAttribute("Type");
		int nSubType = pActivityElem->IntAttribute("SubType");
		if (nID <= 0 || nID >= MAX_SERVER_ACT_NUM)
		{
			continue;
		}
		CServerActivity* pActivity = mServerActivity[nID];
		if (NULL == pActivity || !pActivity->getAvail())
		{
			return;
		}

		pActivity->loadActivity(pActivityElem);
		pActivity->start();
	}
}

/// 根据活动类型创建开服活动
CServerActivity* CServerActModule::createServerActivity(EmSvrActType emSvrActType)
{
	CServerActivity* pActivity = NULL;
	switch (emSvrActType)
	{
		case emSvrActType_CumRecharge:
		{
			pActivity = new CCumulativeRecharge;
			break;
		}
		case emSvrActType_CumConsume:
		{
			pActivity = new CCumulativeConsume;
			break;
		}
		case emSvrActType_Rank:
		{
			pActivity = new CRankActivity;
			break;
		}
		default:
		{
			break;
		}
	}

	return pActivity;
}

/// 清算结束的活动
void CServerActModule::clearEndedActivity()
{
	time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
	for (unsigned int i = 0; i < MAX_SERVER_ACT_NUM; ++ i)
	{
		if (NULL == mServerActivity[i] || !mServerActivity[i]->getAvail())
		{
			continue;
		}

		// 已经结束
		if (mServerActivity[i]->getEndTime() <= tTimeNow)
		{
			mServerActivity[i]->end();
		}
	}
}


CServerActivity* CServerActModule::getServerActivity(int nActivityID)
{
	if (nActivityID <= 0 || nActivityID >= MAX_SERVER_ACT_NUM)
	{
		return NULL;
	}

	return mServerActivity[nActivityID];
}

/// 刷新活动进度
void CServerActModule::refreshProcess(EmSvrActType eType, CEntityPlayer& rPlayer, int nParam1, int nParam2)
{
	for (int i = 0; i < mAvailActivityNum; ++ i)
	{
		if (mAvailActivity[i]->getType() != eType)
		{
			continue;
		}
		mAvailActivity[i]->refreshProcess(rPlayer, nParam1, nParam2);
	}
}