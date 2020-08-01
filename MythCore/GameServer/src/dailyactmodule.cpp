#include "dailyactmodule.h"
#include "scenejob.h"
#include "locallogjob.h"
#include "timemanager.h"
#include "dailyactmodule.hxx.pb.h"
CDailyActModule::CDailyActModule()
{
	init();
}

CDailyActModule::~CDailyActModule()
{
	clear();
}

/// ����������
void CDailyActModule::onLaunchServer()
{
	loadActivityConfig("gameserverconfig/daily_activity/daily_activity.xml");
	checkPassedActivity();
}

/// ������ɼ��
bool CDailyActModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CDailyActModule::onLaunchComplete()
{

}

/// �˳�������
void CDailyActModule::onExitServer()
{

}

/// ��һ�쵽��
void CDailyActModule::onNewDayCome()
{
	loadActivityConfig("gameserverconfig/daily_activity/daily_activity.xml");
}

/// ��һ�ܵ���
void CDailyActModule::onNewWeekCome()
{

}

/// ����ʵ��
void CDailyActModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
void CDailyActModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CDailyActModule::onTimer(unsigned int nTickOffset)
{
	// û����һ���Ƿ���ͬһ����
	if (mLastTime != CTimeManager::Inst()->getCurrTime())
	{
		tm& tmNow = CSceneJob::Inst()->getTmNow();
		// ÿ���Ӽ��һ��
		if (tmNow.tm_min != mLastMinute)
		{
			checkActivityTime();
			mLastMinute = tmNow.tm_min;
		}

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

/// ���ػ����
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
	bool bAlreadyLoad[emDailyActTypeMax] = { false };
	char acPathFile[emDailyActTypeMax][STR_LENGTH_256] = { 0 };

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
				// ����������Ǿ���Ҫ����꣬����ֻ��Ҫ�������
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
		EmDailyActType emActType = (EmDailyActType)pActivityElem->IntAttribute("Type");
		CDailyActivity* pActivity = createDailyActivity(emActType);
		if (NULL == pActivity)
		{
			LOG_ERROR("create daily activity failure, Type: %d, ID: %d", emActType, nActivityID);
			continue;
		}
		pActivity->SetStatus(emDailyActStatus_None);
		pActivity->mID = nActivityID;
		pActivity->mType = emActType;
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


		if (!bAlreadyLoad[emActType] && NULL != pActivity->getConfigFileName())
		{
			bAlreadyLoad[emActType] = true;
			snprintf(acPathFile[emActType], STR_LENGTH_256 - 1, "%s%s", "gameserverconfig/daily_activity/", pActivity->getConfigFileName());
		}
	}

	std::sort(mTimeList.begin(), mTimeList.end());
	for (int nActType = 0; nActType < emDailyActTypeMax; ++ nActType)
	{
		if (bAlreadyLoad[nActType])
		{
			loadSpecifyActivityConfig(acPathFile[nActType]);
		}
	}
}

/// ������ϸ�����
void CDailyActModule::loadSpecifyActivityConfig(const char* pConfigFile)
{
	if (NULL == pConfigFile)
	{
		return;
	}

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
		if (nID < 0  || nID >= MAX_DAILY_ACT_ID)
		{
			continue;
		}
		if (NULL == mActivity[nID])
		{
			continue;
		}
		
		mActivity[nID]->loadActivity(pActivityElem);
	}
}

/// �����
CDailyActivity* CDailyActModule::createDailyActivity(EmDailyActType eType)
{
	CDailyActivity* pActivity = NULL;
	switch (eType)
	{
		case emDailyActType_XXX:
			break;
		default:
			break;
	}

	return pActivity;
}

/// ����Ѿ�����/��ʱ�Ļ
void CDailyActModule::checkPassedActivity()
{
	for (; mTimeListIndex < mTimeList.size(); ++mTimeListIndex)
	{
		if (!checkTimePassed(mTimeList[mTimeListIndex].mTime, CSceneJob::Inst()->getTmNow()))
		{
			break;
		}

		// ֻ������ʼ
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
		// ��Ѿ�����
		if (checkTimePassed(nEndTime, CSceneJob::Inst()->getTmNow()))
		{
			mActivity[nActivityID]->SetStatus(emDailyActStatus_End);
			continue;
		}

		onActivityStart(nActivityID);
		mActivity[nActivityID]->SetStatus(emDailyActStatus_Start);
	}

}

/// ���ʱ��
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
					mActivity[nActivityID]->start();
					onActivityStart(mTimeList[mTimeListIndex].mID);
					mActivity[nActivityID]->SetStatus(emDailyActStatus_Start);
					LOG_INFO("Activity Start : %d", mTimeList[mTimeListIndex].mID);
					break;
				}
				case emDailyActStatus_End:
				{
					mActivity[nActivityID]->end();
					onActivityEnd(mTimeList[mTimeListIndex].mID);
					mActivity[nActivityID]->SetStatus(emDailyActStatus_End);
					LOG_INFO("Activity End : %d", mTimeList[mTimeListIndex].mID);
					break;
				}
				case emDailyActStatus_Notice:
				{
					mActivity[nActivityID]->notice();
					onActivityNotice(mTimeList[mTimeListIndex].mID);
					break;
				}
				default:
				{
					break;
				}
			}

			++ mTimeListIndex;
		}
		else
		{
			break;
		}
	}
}

/// ���ʱ���Ƿ��Ѿ���ȥ(ע�⣺ActTime�ĸ�ʽ�ǣ�1630��ʾ16ʱ30��)
bool CDailyActModule::checkTimePassed(int nActTime, tm& rTm)
{
	int nTime = rTm.tm_hour * 100 + rTm.tm_min;
	if (nTime > nActTime)
	{
		return true;
	}
	return false;
}

void CDailyActModule::onActivityStart(int nActivityID)
{

	sendActivityStatusNotify(nActivityID, emDailyActStatus_Start);
}

void CDailyActModule::onActivityEnd(int nActivityID)
{
	sendActivityStatusNotify(nActivityID, emDailyActStatus_End);
}

void CDailyActModule::onActivityNotice(int nActivityID)
{
	sendActivityStatusNotify(nActivityID, emDailyActStatus_Notice);
}

void CDailyActModule::sendActivityStatusNotify(int nActivityID, int nStatus)
{
	CActivityStatusNotify tActivityStatusNotify;
	tActivityStatusNotify.set_activityid(nActivityID);
	tActivityStatusNotify.set_status(nStatus);

	CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_ACTIVITY_START, &tActivityStatusNotify);
}