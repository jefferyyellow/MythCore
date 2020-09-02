#include "serveractmodule.h"
#include "serveractivity.h"
#include "locallogjob.h"
#include "serverutility.h"
#include "serveractivityunit.h"
#include "entityplayer.h"
#include "scenejob.h"
#include "serveractmodule.hxx.pb.h"
#include "timemanager.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"
#include "curlhandle.h"
#include "platjob.h"
#include "internalmsg.h"
#include "fileutility.h"
CServerActModule::CServerActModule()
{
	init();
	mServerOpenTime = unixTimeStamp("2020-09-01 00:00:00");
}

void CServerActModule::init()
{
	mServerOpenTime = 0;
	mServerStartTime = 0;
	for (int i = 0; i < MAX_SERVER_ACT_NUM; ++i)
	{
		mServerActivity[i] = NULL;
		mAvailActivity[i] = NULL;
	}
	mAvailActivityNum = 0;
	mConfigFileLoadComplete = false;
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
}

/// 启动完成检查
bool CServerActModule::onCheckLaunch()
{
	return mConfigFileLoadComplete;
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

/// 加载配置文件
void CServerActModule::onLoadConfig()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
	lua_tinker::dofile(L, "gameserverconfig/script/server_activity/server_activity.lua");

	sendPlatWebRequest("server_activity.xml", "server_activity.xml", EmHttpType(emHttpTypeGet | emHttpTypeFile), true);
	// loadServerActivityConfig("gameserverconfig/server_activity/server_activity.xml");
}

/// 重新加载配置文件
void CServerActModule::onReloadConfig()
{
	sendPlatWebRequest("server_activity.xml", "server_activity.xml", EmHttpType(emHttpTypeGet | emHttpTypeFile), true);
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
		if (nType <= 0 || nType >= emServerActTypeMax)
		{
			continue;
		}
		CServerActivity* pActivity = new CServerActivity;
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

		// 
		mAvailActivity[mAvailActivityNum] = pActivity;
		++ mAvailActivityNum;
	}

	for (int i = 0; i < mAvailActivityNum; ++ i)
	{
		const char* pFileName = mAvailActivity[i]->getConfigName();
		if (pFileName[0] == '\0')
		{
			continue;
		}
		mConfigFileList.insert(pFileName);
		sendPlatWebRequest(pFileName, pFileName, EmHttpType(emHttpTypeGet | emHttpTypeFile), true);
	}
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

		time_t nEndTime = mServerActivity[i]->getPrizeTime() > mServerActivity[i]->getEndTime() ?
			mServerActivity[i]->getPrizeTime() : mServerActivity[i]->getEndTime();

		// 已经结束
		if (nEndTime <= tTimeNow)
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
	lua_State* L = CSceneJob::Inst()->getLuaState();
	for (int i = 0; i < mAvailActivityNum; ++ i)
	{
		if (mAvailActivity[i]->getType() != eType)
		{
			continue;
		}
		mAvailActivity[i]->refreshProcess(rPlayer, nParam1, nParam2);
	}
}

/// 是否在活动期间
bool CServerActModule::checkActOpen(int nActivityID)
{
	if (nActivityID <= 0 || nActivityID >= MAX_SERVER_ACT_NUM)
	{
		return false;
	}
	if (NULL == mAvailActivity[nActivityID])
	{
		return false;
	}

	time_t tNowTime = CTimeManager::Inst()->getCurrTime();
	if (tNowTime < mAvailActivity[nActivityID]->getStartTime()
		|| tNowTime >= mAvailActivity[nActivityID]->getEndTime())
	{
		return false;
	}

	return true;
}

/// 是否在可以领奖的期间,如果领奖时间大于0，表示是否在活动结束到领奖期间
/// 如果领奖时间小于等于0，表示是否在活动开始到结束期间都能领奖
bool CServerActModule::checkActPrizeTime(int nActivityID)
{
	if (nActivityID <= 0 || nActivityID >= MAX_SERVER_ACT_NUM)
	{
		return false;
	}

	if (NULL == mAvailActivity[nActivityID])
	{
		return false;
	}
	time_t tNowTime = CTimeManager::Inst()->getCurrTime();
	if (mAvailActivity[nActivityID]->getPrizeTime() > 0)
	{
		if (tNowTime < mAvailActivity[nActivityID]->getEndTime()
			|| tNowTime >= mAvailActivity[nActivityID]->getPrizeTime())
		{
			return false;
		}

		return true;
	}
	else
	{
		if (tNowTime < mAvailActivity[nActivityID]->getStartTime()
			|| tNowTime >= mAvailActivity[nActivityID]->getEndTime())
		{
			return false;
		}
		return true;
	}
}

/// 每日刷新所以的玩家
void CServerActModule::dailyRefreshAllPlayer()
{
	CEntityPlayer* pPlayer = NULL;
	CSceneJob::PLAYER_LIST& rPlayerList = CSceneJob::Inst()->getPlayerList();
	CSceneJob::PLAYER_LIST::iterator it = rPlayerList.begin();
	for (; it != rPlayerList.end(); ++it)
	{
		pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
		if (NULL == pPlayer)
		{
			continue;
		}
		if (emPlayerStatus_Gameing != pPlayer->getPlayerStauts())
		{
			continue;
		}
		pPlayer->getServerActUnit().dailyRefresh();
	}
}

/// 加载平台配置文件回调
void CServerActModule::onLoadPlatFile(CIMPlatWebResponse* pResponse)
{
	if (NULL == pResponse)
	{
		return;
	}
	char acFileName[STR_LENGTH_128];
	CFileUtility::GetFileName(pResponse->mReturnData, acFileName, STR_LENGTH_128 - 1);

	char acExtension[STR_LENGTH_16];
	CFileUtility::GetExtension(acFileName, acExtension, STR_LENGTH_16 - 1);

	bool bConfigFileList = true;
	if (strncmp("server_activity.xml", acFileName, RETURN_DATA_LENGTH) == 0)
	{
		CServerActModule::Inst()->loadServerActivityConfig(pResponse->mReturnData);
		bConfigFileList = false;
	}

	if (!bConfigFileList)
	{
		return;
	}

	lua_State* L = CSceneJob::Inst()->getLuaState();
	if (strncmp("lua", acExtension, sizeof(acExtension) - 1) == 0)
	{
		if (luaL_loadfile(L, pResponse->mReturnData) != LUA_OK)
		{
			LOG_ERROR("lua format error, File Name: %s", acFileName);
			return;
		}
		char acPathFileName[STR_LENGTH_128];
		CFileUtility::GetFilePathWithoutExtension(pResponse->mReturnData, acPathFileName, STR_LENGTH_128 - 1);
		lua_tinker::call<int>(L, "Global_reloadFile", acPathFileName);
	}

	mConfigFileList.erase(acFileName);
	if (mConfigFileList.size() <= 0)
	{
		// 加载完成
		mConfigFileLoadComplete = true;
	}
}
