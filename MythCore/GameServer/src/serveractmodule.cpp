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

/// ����������
void CServerActModule::onLaunchServer()
{
}

/// ������ɼ��
bool CServerActModule::onCheckLaunch()
{
	return mConfigFileLoadComplete;
}

/// �����������ɹ�
void CServerActModule::onLaunchComplete()
{
	mServerStartTime = CTimeManager::Inst()->getCurrTime();
}

/// �˳�������
void CServerActModule::onExitServer()
{

}

/// ��һ�쵽��
void CServerActModule::onNewDayCome()
{
	// ��������Ļ
	clearEndedActivity();
	// �����µ�һ��Ļ
	loadServerActivityConfig("gameserverconfig/server_activity/server_activity.xml");
}

/// ��һ�ܵ���
void CServerActModule::onNewWeekCome()
{

}

/// ����ʵ��
void CServerActModule::onCreatePlayer(CEntityPlayer* pPlayer)
{
	
}

/// ����ʵ��
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

/// ���������ļ�
void CServerActModule::onLoadConfig()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
	lua_tinker::dofile(L, "gameserverconfig/script/server_activity/server_activity.lua");

	sendPlatWebRequest("server_activity.xml", "server_activity.xml", EmHttpType(emHttpTypeGet | emHttpTypeFile), true);
	// loadServerActivityConfig("gameserverconfig/server_activity/server_activity.xml");
}

/// ���¼��������ļ�
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

	// ���
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

		// ������ڻ�ڼ��಻�����Ӧ�������ļ���ֻ���ظ�Ҫ��Ϣ
		// �����ǰʱ��ȿ�ʼʱ�仹�磬���ߵ�ǰʱ��Ƚ���ʱ�仹��
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

/// ��������Ļ
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

		// �Ѿ�����
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

/// ˢ�»����
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

/// �Ƿ��ڻ�ڼ�
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

/// �Ƿ��ڿ����콱���ڼ�,����콱ʱ�����0����ʾ�Ƿ��ڻ�������콱�ڼ�
/// ����콱ʱ��С�ڵ���0����ʾ�Ƿ��ڻ��ʼ�������ڼ䶼���콱
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

/// ÿ��ˢ�����Ե����
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

/// ����ƽ̨�����ļ��ص�
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
		// �������
		mConfigFileLoadComplete = true;
	}
}
