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
CServerActModule::CServerActModule()
{
	init();
	mServerOpenTime = unixTimeStamp("2020-8-4 00:00:00");
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
	lua_State* L = CSceneJob::Inst()->getLuaState();
	lua_tinker::dofile(L, "gameserverconfig/script/server_activity/server_activity.lua");
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

	bool bAlreadyLoad[emServerActTypeMax] = { false };
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

		// 所以
		mAvailActivity[mAvailActivityNum] = pActivity;
		++ mAvailActivityNum;
		bAlreadyLoad[nType] = true;
	}

	lua_State* L = CSceneJob::Inst()->getLuaState();
	for (int i = 0; i < emServerActTypeMax; ++ i)
	{
		if (bAlreadyLoad[i])
		{
			lua_tinker::call<int>(L, "ServerActivity_initFunc", i);
		}
	}

}

/// 清算结束的活动
void CServerActModule::clearEndedActivity()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
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
			lua_tinker::call<int>(L, mServerActivity[i]->getEndFunc());
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
		lua_tinker::call<int>(L, mAvailActivity[i]->getRefreshProcess(), 
			mAvailActivity[i]->getID(), &rPlayer, nParam1, nParam2);
	}
}

/// 初始化类型函数
void CServerActModule::initActivityTypeFunc(int nType, int nFuncType, const char* pFuncName)
{
	if (NULL == pFuncName)
	{
		return;
	}
	for (int i = 0; i < mAvailActivityNum; ++i)
	{
		if (mAvailActivity[i]->getType() != nType)
		{
			continue;
		}
		switch (nFuncType)
		{
			case emFuncType_End:
			{
				mAvailActivity[i]->setEndFunc(pFuncName);
				break;
			}
			case emFuncType_ClearPlayerData:
			{
				mAvailActivity[i]->setClearPlayerData(pFuncName);
				break;
			}
			case emFuncType_RefreshProcess:
			{
				mAvailActivity[i]->setRefreshProcess(pFuncName);
				break;
			}
		}
	}

}
