#include "serveractmodule.h"
#include "serveractivity.h"
#include "locallogjob.h"
#include "serverutility.h"
#include "timemanager.h"
#include "serveractivityunit.h"
#include "entityplayer.h"
#include "scenejob.h"
#include "serveractmodule.hxx.pb.h"
CServerActModule::CServerActModule()
{
	mServerStartTime = unixTimeStamp("2018-2-17 00:00:00");
	init();
}


CServerActModule::~CServerActModule()
{

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
	dailyRefresh(pPlayer);
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
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	sendGetServerActResponse(pPlayer);
}

void CServerActModule::sendGetServerActResponse(CEntityPlayer* pPlayer)
{
	MYTH_ASSERT(NULL == pPlayer, return);

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
			nStartTime = mServerStartTime + (nStartDay - 1) * SECONDS_PER_DAY;
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
		// 如果当前时间比开始时间还早，或者当前时间比结束时间还大
		if (nTimeNow < nStartTime || (nTimeNow >(nPrizeTime > nEndTime ? nPrizeTime : nEndTime)))
		{
			continue;
		}

		int nType = pActivityElem->IntAttribute("Type");
		CServerActivity* pActivity = createServerActivity((EmServerActType)nType);
		if (NULL == pActivity)
		{
			LOG_ERROR("Create server activity failure, %d", nType);
			continue;
		}

		pActivity->setType(nType);
		pActivity->setSubType(pActivityElem->IntAttribute("SubType"));
		pActivity->setID(pActivityElem->IntAttribute("ID"));
		pActivity->setStartTime(nStartTime);
		pActivity->setEndTime(nEndTime);
		pActivity->setPrizeTime(nPrizeTime);

		mServerActList.push_back(pActivity);
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
		int nType = pActivityElem->IntAttribute("Type");
		int nSubType = pActivityElem->IntAttribute("SubType");
		int nID = pActivityElem->IntAttribute("ID");

		CServerActivity* pActivity = getServerActivity(nType, nSubType, nID);
		if (NULL == pActivity)
		{
			return;
		}

		pActivity->loadActivity(pActivityElem);
	}
}

CServerActivity* CServerActModule::createServerActivity(EmServerActType emServerActType)
{
	CServerActivity* pActivity = NULL;
	switch (emServerActType)
	{
		case emServerActType_Phase:
		{
			pActivity = new CPhaseActivity;
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
	for (unsigned int i = 0; i < mServerActList.size(); ++ i)
	{
		// 已经结束
		if (mServerActList[i]->getEndTime() < tTimeNow)
		{
			mServerActList[i]->end();
		}
	}
}

/// 每日刷新
void CServerActModule::dailyRefresh(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	// 如果上次下线的时间在今天早上以后了，那就是同一天的多次登录了，并且服务器重启以后，玩家已经登录过了
	if (pPlayer->getLastOffTime() >= CSceneJob::Inst()->getMorningTime() && pPlayer->getLastOffTime() >= mServerStartTime)
	{
		return;
	}

	CServerActivityUnit& rUnit = pPlayer->getServerActUnit();
	rUnit.checkAllServerAct();

	for (unsigned int i = 0; i < mServerActList.size(); ++ i)
	{
		int nUniqueID = MAKE_ACT_UNIQUE_ID(mServerActList[i]->getType(), mServerActList[i]->getSubType(), mServerActList[i]->getID());
		CServerActData* pActData = rUnit.getServerActData(nUniqueID);
		if (NULL == pActData)
		{
			CServerActData tData;
			tData.setUniqueID(nUniqueID);
			tData.setStartTime(mServerActList[i]->getStartTime());
			rUnit.addServerActData(tData);
		}
	}
}

CServerActivity* CServerActModule::getServerActivity(int nActUniqueID)
{
	for (unsigned int i = 0; i < mServerActList.size(); ++ i)
	{
		int nUniqueID = MAKE_ACT_UNIQUE_ID(mServerActList[i]->getType(), mServerActList[i]->getSubType(), mServerActList[i]->getID());
		if (nUniqueID == nActUniqueID)
		{
			return mServerActList[i];
		}
	}

	return NULL;
}

CServerActivity* CServerActModule::getServerActivity(int nType, int nSubType, int nID)
{
	for (unsigned int i = 0; i < mServerActList.size(); ++i)
	{
		if (mServerActList[i]->getType() == nType 
			&& mServerActList[i]->getSubType() == nSubType 
			&& mServerActList[i]->getID() == nID)
		{
			return mServerActList[i];
		}
	}

	return NULL;
}
