#include "gameserverconfig.h"
#include "tinyxml2.h"
using namespace tinyxml2;
void CGameServerConfig::loadGameServerConfigFromXml(char* pFilePath)
{
	if (NULL == pFilePath)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument; 
	if (XML_SUCCESS != tDocument.LoadFile(pFilePath))
	{
		// 出错，无法加载xml文件
		return;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// 出错，连Root节点都没有
		return;
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

}