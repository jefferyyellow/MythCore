#include "GameLogicConfig.h"
#include "locallogjob.h"
bool CGameLogicConfig::loadGameLogicConfig(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		LOG_ERROR("load game logic config failure, %s", pFilePath);
		return false;
	}

	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pFilePath))
	{
		// �����޷�����xml�ļ�
		LOG_ERROR("load game logic config failure, %s", pFilePath);
		return false;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// ������Root�ڵ㶼û��
		LOG_ERROR("game logic config has no root element, %s", pFilePath);
		return false;
	}

	XMLElement* pHeartBeatElem = pRoot->FirstChildElement("HeartBeat");
	if (NULL != pHeartBeatElem)
	{
		mHeartBeatCheck = (bool)pHeartBeatElem->BoolAttribute("Check");
		mHeartBeatTime = pHeartBeatElem->IntAttribute("Time");
		mHeartBeatCheckTime = pHeartBeatElem->IntAttribute("CheckTime");
		mHeartBeatInvalidMax = pHeartBeatElem->IntAttribute("InvalidMax");
	}

	return true;
}