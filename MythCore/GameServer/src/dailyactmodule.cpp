#include "dailyactmodule.h"
#include "scenejob.h"
/// ����������
void CDailyActModule::onLaunchServer()
{
	loadActivityConfig("gameserverconfig/DailyActivity.xml");
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

/// ���ػ����
void CDailyActModule::loadActivityConfig(const char* pConfigFile)
{
	if (NULL == pConfigFile)
	{
		return;
	}

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
				if ((tYear == tTempTm.tm_year + 1900) && (tMonth == tTempTm.tm_mon + 1) && (tDay == tTempTm.tm_mday))
				{
					bDate = true;
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


		int nCount = 0;
		XMLElement* pNoticeElem = pActivityElem->FirstChildElement("Notice");
		for (; NULL != pNoticeElem; pNoticeElem = pNoticeElem->NextSiblingElement("Notice"))
		{

		}

		nCount = 0;
		XMLElement* pTimeElem = pActivityElem->FirstChildElement("Time");
		for (; NULL != pTimeElem; pTimeElem = pTimeElem->NextSiblingElement("Time"))
		{

		}

	}
}