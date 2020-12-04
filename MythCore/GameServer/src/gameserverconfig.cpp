#include "gameserverconfig.h"
#include "locallogjob.h"
bool CGameServerConfig::loadGameServerConfigFromXml(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		LOG_ERROR("load game server config failure, %s", pFilePath);
		return false;
	}
	tinyxml2::XMLDocument tDocument; 
	if (XML_SUCCESS != tDocument.LoadFile(pFilePath))
	{
		// 出错，无法加载xml文件
		LOG_ERROR("load game server config failure, %s", pFilePath);
		return false;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// 出错，连Root节点都没有
		LOG_ERROR("game server config file has no root element, %s", pFilePath);
		return false;
	}

	XMLElement* pDBInfo = pRoot->FirstChildElement("DBInfo");
	if (NULL != pDBInfo)
	{
		XMLElement* pHostElement = pDBInfo->FirstChildElement("Host");
		if (NULL != pHostElement)
		{
			const char* pHost = pHostElement->Attribute("value");
			if (NULL != pHost)
			{
				strncpy(mDBHost, pHost, sizeof(mDBHost) - 1);
			}
		}

		XMLElement* pUserNameElement = pDBInfo->FirstChildElement("UserName");
		if (NULL != pUserNameElement)
		{
			const char* pUserName = pUserNameElement->Attribute("value");
			if (NULL != pUserName)
			{
				strncpy(mDBUserName, pUserName, sizeof(mDBUserName) - 1);
			}
		}
		
		XMLElement* pPassWordElement = pDBInfo->FirstChildElement("PassWord");
		if (NULL != pPassWordElement)
		{
			const char* pPassWord = pPassWordElement->Attribute("value");
			if (NULL != pPassWord)
			{
				strncpy(mDBPasswd, pPassWord, sizeof(mDBPasswd) - 1);
			}
		}

		XMLElement* pDefaultDBElement = pDBInfo->FirstChildElement("DefaultDataBase");
		if (NULL != pDefaultDBElement)
		{
			const char* pDefaultDB = pDefaultDBElement->Attribute("value");
			if (NULL != pDefaultDB)
			{
				strncpy(mDefaultDataBase, pDefaultDB, sizeof(mDefaultDataBase) - 1);
			}
		}

		XMLElement* pPortElement = pDBInfo->FirstChildElement("Port");
		if (NULL != pPortElement)
		{
			mDBPort = pPortElement->IntAttribute("value");
		}
	}

	XMLElement* pNetInfoElem = pRoot->FirstChildElement("NetInfo");
	if (NULL != pNetInfoElem)
	{
		XMLElement* pPortElement = pNetInfoElem->FirstChildElement("Port");
		if (NULL != pPortElement)
		{
			mListenPort = pPortElement->IntAttribute("value");
		}
	}

	XMLElement* pServerInfoElem = pRoot->FirstChildElement("ServerInfo");
	if (NULL != pServerInfoElem)
	{
		XMLElement* pServerIDElem = pServerInfoElem->FirstChildElement("ServerID");
		if (NULL != pServerIDElem)
		{
			mServerID = pServerIDElem->IntAttribute("value");
		}
	}

	return true;
}