#include "dailyactmodule.h"
#include "scenejob.h"
#include "locallogjob.h"
#include "dailyactmodule.hxx.pb.h"
#include "timemanager.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"
CDailyActModule::CDailyActModule()
{
	init();
}

CDailyActModule::~CDailyActModule()
{
	clear();
}

/// 启动服务器
void CDailyActModule::onLaunchServer()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
	lua_tinker::dofile(L, "gameserverconfig/script/daily_activity/daily_activity.lua");

	loadActivityConfig("gameserverconfig/daily_activity/daily_activity.xml");
	checkPassedActivity();
}

/// 启动完成检查
bool CDailyActModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CDailyActModule::onLaunchComplete()
{

}

/// 退出服务器
void CDailyActModule::onExitServer()
{

}

/// 新一天到来
void CDailyActModule::onNewDayCome()
{
	loadActivityConfig("gameserverconfig/daily_activity/daily_activity.xml");
}

/// 新一周到来
void CDailyActModule::onNewWeekCome()
{

}

/// 建立实体
void CDailyActModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
void CDailyActModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CDailyActModule::onTimer(unsigned int nTickOffset)
{
	// 是不是同一秒钟
	if (mLastTime != CTimeManager::Inst()->getCurrTime())
	{
		checkActivityTime();
		mLastTime = CTimeManager::Inst()->getCurrTime();
	}
}

void CDailyActModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	switch (nMessageID)
	{

	}
}

bool operator < (const CDailyActTime& rLeft, const CDailyActTime& rRight)
{
	return rLeft.mTime < rRight.mTime;
}

/// 加载活动配置
void CDailyActModule::loadActivityConfig(const char* pConfigFile)
{
	if (NULL == pConfigFile)
	{
		return;
	}

	clear();

	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pConfigFile))
	{
		LOG_ERROR("Load daily activity config file failure!");
		return;
	}

	XMLElement* pRootElem = tDocument.RootElement();
	if (NULL == pRootElem)
	{
		LOG_ERROR("Don't find daily activity config root element");
		return;
	}

	tm tTempTm = CSceneJob::Inst()->getTmNow();
	XMLElement* pActivityElem = pRootElem->FirstChildElement("Activity");
	for (; NULL != pActivityElem; pActivityElem = pActivityElem->NextSiblingElement("Activity"))
	{

		XMLElement* pChildElem = pActivityElem->FirstChildElement("Date");
		if (NULL != pChildElem)
		{
			bool bDate = false;
			const char* pDate = pChildElem->Attribute("Date");
			if (NULL != pDate)
			{
				int nDate = atoi(pDate);
				int tYear = nDate / 10000;
				int tMonth = (nDate % 10000) / 100;
				int tDay = nDate % 100;
				// 如果填了年那就需要检测年，或者只需要检测日期
				if (tYear > 0)
				{
					if ((tYear == tTempTm.tm_year + 1900) && (tMonth == tTempTm.tm_mon + 1) && (tDay == tTempTm.tm_mday))
					{
						bDate = true;
					}
				}
				else
				{
					if ((tMonth == tTempTm.tm_mon + 1) && (tDay == tTempTm.tm_mday))
					{
						bDate = true;
					}
				}
			}

			const char* pWeekDay = pChildElem->Attribute("WeekDay");
			if (NULL != pWeekDay)
			{
				int nWeekDay = atoi(pWeekDay);
				if (nWeekDay != 0 && (nWeekDay == tTempTm.tm_wday || (nWeekDay == 7 && tTempTm.tm_wday == 0)))
				{
					bDate = true;
				}
			}

			const char* pMonthDay = pChildElem->Attribute("MonthDay");
			if (NULL != pMonthDay)
			{
				int nMonthDay = atoi(pMonthDay);
				if (nMonthDay != 0 && nMonthDay == tTempTm.tm_mday)
				{
					bDate = true;
				}
			}

			if (!bDate)
			{
				continue;
			}
		}

		int nActivityID = pActivityElem->IntAttribute("ID");
		if (nActivityID < 0 || nActivityID >= MAX_DAILY_ACT_ID)
		{
			LOG_ERROR("activity id is valid, %d", nActivityID);
			continue;
		}

		CDailyActivity* pActivity = new CDailyActivity;
		if (NULL == pActivity)
		{
			LOG_ERROR("create daily activity failure, ID: %d", nActivityID);
			continue;
		}

		mActivity[nActivityID] = pActivity;
		pActivity->SetStatus(emDailyActStatus_None);
		pActivity->mID = nActivityID;
		pActivity->mType = (EmDailyActType)pActivityElem->IntAttribute("Type");
		pActivity->mMinLevel = pActivityElem->IntAttribute("LevelMin");
		pActivity->mMaxLevel = pActivityElem->IntAttribute("LevelMax");

		int nCount = 0;
		CDailyActTime tActivityTime;
		tActivityTime.mID = pActivity->mID;
		XMLElement* pTimeElem = pActivityElem->FirstChildElement("Time");
		for (; NULL != pTimeElem; pTimeElem = pTimeElem->NextSiblingElement("Time"))
		{
			tActivityTime.mTimeIndex = nCount;

			pActivity->mStartTime[nCount] = pTimeElem->IntAttribute("StartTime");
			tActivityTime.mTime = pActivity->mStartTime[nCount];
			tActivityTime.mStatus = emDailyActStatus_Start;
			mTimeList.push_back(tActivityTime);

			pActivity->mEndTime[nCount] = pTimeElem->IntAttribute("EndTime");
			tActivityTime.mTime = pActivity->mEndTime[nCount];
			tActivityTime.mStatus = emDailyActStatus_End;
			mTimeList.push_back(tActivityTime);

			pActivity->mNoticeTime[nCount] = pTimeElem->IntAttribute("NoticeTime");
			tActivityTime.mTime = pActivity->mNoticeTime[nCount];
			tActivityTime.mStatus = emDailyActStatus_Notice;
			mTimeList.push_back(tActivityTime);
			++ nCount;
		}
	}

	std::sort(mTimeList.begin(), mTimeList.end());
}

/// 检测已经开启/过时的活动
void CDailyActModule::checkPassedActivity()
{
	for (; mTimeListIndex < mTimeList.size(); ++mTimeListIndex)
	{
		if (!checkTimePassed(mTimeList[mTimeListIndex].mTime, CSceneJob::Inst()->getTmNow()))
		{
			break;
		}

		// 只处理活动开始,并且对其对应的结束时间进行校验
		if (mTimeList[mTimeListIndex].mStatus != emDailyActStatus_Start)
		{
			continue;
		}
		
		int nActivityID = mTimeList[mTimeListIndex].mID;
		if (nActivityID <= 0 || nActivityID >= MAX_DAILY_ACT_ID)
		{
			continue;
		}

		int nTimeIndex = mTimeList[mTimeListIndex].mTimeIndex;
		if (nTimeIndex < 0 || nTimeIndex >= MAX_ACTIVITY_TIMES)
		{
			continue;
		}
		if (NULL == mActivity[nActivityID])
		{
			continue;
		}

		int nEndTime = mActivity[nActivityID]->mEndTime[nTimeIndex];
		// 活动已经结束
		if (checkTimePassed(nEndTime, CSceneJob::Inst()->getTmNow()))
		{
			mActivity[nActivityID]->SetStatus(emDailyActStatus_End);
			continue;
		}
		onActivityStart(nActivityID);
	}

}

/// 检测活动时间
void CDailyActModule::checkActivityTime()
{
	for (; mTimeListIndex < mTimeList.size(); ++ mTimeListIndex)
	{
		if (checkTimePassed(mTimeList[mTimeListIndex].mTime, CSceneJob::Inst()->getTmNow()))
		{
			int nActivityID = mTimeList[mTimeListIndex].mID;
			if (nActivityID <= 0 || nActivityID >= MAX_DAILY_ACT_ID)
			{
				continue;
			}
			if (NULL == mActivity[nActivityID])
			{
				continue;
			}

			switch (mTimeList[mTimeListIndex].mStatus)
			{
				case emDailyActStatus_Start:
				{
					onActivityStart(mTimeList[mTimeListIndex].mID);
					LOG_INFO("Activity Start : %d", mTimeList[mTimeListIndex].mID);
					break;
				}
				case emDailyActStatus_End:
				{
					onActivityEnd(mTimeList[mTimeListIndex].mID);
					LOG_INFO("Activity End : %d", mTimeList[mTimeListIndex].mID);
					break;
				}
				case emDailyActStatus_Notice:
				{
					onActivityNotice(mTimeList[mTimeListIndex].mID);
					break;
				}
				default:
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
}

/// 检测时间是否已经过去(注意：ActTime的格式是：1630表示16时30分)
bool CDailyActModule::checkTimePassed(int nActTime, tm& rTm)
{
	int nTime = rTm.tm_hour * 100 + rTm.tm_min;
	if (nTime >= nActTime)
	{
		return true;
	}
	return false;
}

void CDailyActModule::onActivityStart(int nActivityID)
{
	if (nActivityID <= 0 || nActivityID >= MAX_DAILY_ACT_ID)
	{
		return;
	}
	if (NULL == mActivity[nActivityID])
	{
		return;
	}

	mActivity[nActivityID]->start();
	mActivity[nActivityID]->SetStatus(emDailyActStatus_Start);
	sendActivityStatusNotify(nActivityID, emDailyActStatus_Start);
}

void CDailyActModule::onActivityEnd(int nActivityID)
{
	if (nActivityID <= 0 || nActivityID >= MAX_DAILY_ACT_ID)
	{
		return;
	}
	if (NULL == mActivity[nActivityID])
	{
		return;
	}

	mActivity[nActivityID]->end();
	mActivity[nActivityID]->SetStatus(emDailyActStatus_End);
	sendActivityStatusNotify(nActivityID, emDailyActStatus_End);
}

void CDailyActModule::onActivityNotice(int nActivityID)
{
	if (nActivityID <= 0 || nActivityID >= MAX_DAILY_ACT_ID)
	{
		return;
	}
	if (NULL == mActivity[nActivityID])
	{
		return;
	}

	mActivity[nActivityID]->notice();
	sendActivityStatusNotify(nActivityID, emDailyActStatus_Notice);
}

void CDailyActModule::sendActivityStatusNotify(int nActivityID, int nStatus)
{
	CActivityStatusNotify tActivityStatusNotify;
	tActivityStatusNotify.set_activityid(nActivityID);
	tActivityStatusNotify.set_status(nStatus);

	CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_ACTIVITY_START, &tActivityStatusNotify);
}