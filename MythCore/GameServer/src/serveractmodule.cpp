#include "serveractmodule.h"
#include "serveractivity.h"
#include "locallogjob.h"


/// ����������
void CServerActModule::onLaunchServer()
{

}

/// ������ɼ��
bool CServerActModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CServerActModule::onLaunchComplete()
{

}

/// �˳�������
void CServerActModule::onExitServer()
{

}

/// ��һ�쵽��
void CServerActModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CServerActModule::onNewWeekCome()
{

}

/// ����ʵ��
void CServerActModule::onCreatePlayer(CEntity* pEntity)
{

}

/// ����ʵ��
void CServerActModule::onDestroyPlayer(CEntity* pEntity)
{

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

	}
}

time_t CServerActModule::timeToMorning(time_t tTime)
{
#ifdef MYTH_OS_WINDOWS
	struct tm tTmNow;
	localtime_s(&tTmNow, &tTime);
#else
	localtime_r(&tTime, &tTmNow);
#endif // MYTH_OS_WINDOWS
	int tSeconds = tTmNow.tm_hour * 60 * 60 + tTmNow.tm_min * 60 + tTmNow.tm_sec;
	return tTime - tSeconds;
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

	XMLElement* pActivityElem = pRootElem->FirstChildElement("Activity");
	for (; NULL != pActivityElem; pActivityElem->NextSiblingElement("Activity"))
	{
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

		int nStartDay = pActivityElem->IntAttribute("StartDay");
		int nDurationDay = pActivityElem->IntAttribute("DurationDay");
		if (nStartDay > 0 && nDurationDay > 0)
		{
			
		}
		else
		{
			int nStartTime = pActivityElem->IntAttribute("StartTime");
			int nEndTime = pActivityElem->IntAttribute("EndTime");
			
		}
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
			break;
	}

	return pActivity;
}