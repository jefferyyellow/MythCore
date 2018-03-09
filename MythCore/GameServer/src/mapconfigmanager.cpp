#include "mapconfigmanager.h"
#include "locallogjob.h"
#include "errcode.h"
#include "mapmamager.h"
#include "entitycreator.h"
bool CMapConfig::loadMapConfig(unsigned short nMapID)
{
	char szFilePath[STR_LENGTH_256] = {0};
	snprintf(szFilePath, sizeof(szFilePath) - 1, "gameserverconfig/map/%d.xml", nMapID);

	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(szFilePath))
	{
		LOG_ERROR("load map configure failure, %s", szFilePath);
		return false;
	}

	XMLElement* pRootElement = tDocument.RootElement();
	if (NULL == pRootElement)
	{
		LOG_ERROR("map configure has no root element, %s", szFilePath);
		return false;
	}

	XMLElement* pDoorElement = pRootElement->FirstChildElement("door");
	if (NULL != pDoorElement)
	{
		XMLElement* pDoorChildElement = pDoorElement->FirstChildElement("d");
		for (; NULL != pDoorChildElement; pDoorChildElement = pDoorChildElement->NextSiblingElement("d"))
		{
			int nX = pDoorChildElement->IntAttribute("x");
			int nY = pDoorChildElement->IntAttribute("y");
			const char* pText = pDoorChildElement->GetText();
			int nMapID = 0;
			if (NULL != pText)
			{
				nMapID = atoi(pText);
			}
			
			// 传送门
			if (nMapID > 0)
			{
				CMapPortalConfig tMapPortalConfig;
				tMapPortalConfig.mPos = CMythPoint(nX/MAP_UNIT_SIZE, nY/MAP_UNIT_SIZE);
				tMapPortalConfig.mTargetMapID = nMapID;
				mMapPortal.push_back(tMapPortalConfig);
			}
			else
			{
				mRelievePos.push_back(CMythPoint(nX/MAP_UNIT_SIZE, nY/MAP_UNIT_SIZE));
			}
		}
	}

	return true;
}

/// 加载地图NPC配置
bool CMapConfig::loadMapNPCConfig(unsigned short nMapID)
{

	char szFilePath[STR_LENGTH_256] = { 0 };
	snprintf(szFilePath, sizeof(szFilePath) - 1, "gameserverconfig/map/%d_monster.xml", nMapID);

	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(szFilePath))
	{
		LOG_ERROR("load map configure failure, %s", szFilePath);
		return false;
	}

	XMLElement* pRootElement = tDocument.RootElement();
	if (NULL == pRootElement)
	{
		LOG_ERROR("map configure has no root element, %s", szFilePath);
		return false;
	}

	XMLElement* pMonsterElement = pRootElement->FirstChildElement("m");
	for (; NULL != pMonsterElement; pMonsterElement = pMonsterElement->NextSiblingElement("m"))
	{
		CMapNPCConfig tNPCConfig;
		XMLElement* pIDElement = pMonsterElement->FirstChildElement("id");
		if (NULL != pIDElement)
		{
			const char* pID = pIDElement->GetText();
			if (NULL != pID)
			{
				tNPCConfig.mTempID = atoi(pID);
			}
		}

		XMLElement* pPosXElement = pMonsterElement->FirstChildElement("x");
		if (NULL != pPosXElement)
		{
			const char* pPosX = pPosXElement->GetText();
			if (NULL != pPosX)
			{
				tNPCConfig.mPos.mX = atoi(pPosX) / MAP_UNIT_SIZE;
			}
		}

		XMLElement* pPosYElement = pMonsterElement->FirstChildElement("y");
		if (NULL != pPosYElement)
		{
			const char* pPosY = pPosYElement->GetText();
			if (NULL != pPosY)
			{
				tNPCConfig.mPos.mY = atoi(pPosY) / MAP_UNIT_SIZE;
			}
		}

		mMapNPC.push_back(tNPCConfig);
	}

	return true;
}

/// 从配置创建地图
int CMapConfig::createMapFromConfig(CMap* pMap)
{
	if (NULL == pMap)
	{
		return -1;
	}
	CNpcCreator		tNpcCreator;
	COgreCreator	tOgreCreator;
	// 创建NPC
	for (unsigned int nIndex = 0; nIndex < mMapNPC.size(); ++ nIndex)
	{
		CTemplate* pTemplate = static_cast<CTemplate*>(CStaticData::searchTpl(mMapNPC[nIndex].mTempID));
		if (NULL == pTemplate)
		{
			continue;
		}
		CEntityCreator* pCreator = NULL;
		
		if (emTemplateType_FuncNpc == pTemplate->mTemplateType)
		{
			pCreator = &tNpcCreator;
		}
		else if (emTemplateType_Ogre == pTemplate->mTemplateType)
		{
			pCreator = &tOgreCreator;
		}

		pCreator->mTempID = mMapNPC[nIndex].mTempID;
		pCreator->mPos = mMapNPC[nIndex].mPos;

		pMap->createEntity(pCreator);
	}

	return SUCCESS;
}

int CMapConfig::getPortalPos(unsigned short nMapID, CMythPoint& rPortalPos)
{
	for (unsigned int i = 0; i < mMapPortal.size(); ++ i)
	{
		if (mMapPortal[i].mTargetMapID == nMapID)
		{
			rPortalPos = mMapPortal[i].mPos;
			return SUCCESS;
		}
	}

	return -1;
}

/// 创建地图
int CMapConfigManager::createMapFromConfig(CMap* pMap)
{
	if (NULL == pMap)
	{
		return -1;
	}
	CMapConfig* pMapConfig = getMapConfig(pMap->getMapID());
	if (NULL == pMapConfig)
	{
		return -1;
	}
	pMapConfig->createMapFromConfig(pMap);

	return SUCCESS;
}

/// 创建所有地图
bool CMapConfigManager::createAllMapFromConfig()
{
	for (int i = 0; i < MAX_MAP_ID; ++ i)
	{
		if (NULL == mMapConfig[i])
		{
			continue;
		}
		CMap* pMap = CMapManager::Inst()->createMap(1, i, 0, mMapConfig[i]->getLength(), mMapConfig[i]->getWidth());
		if (NULL == pMap)
		{
			return false;
		}

		createMapFromConfig(pMap);
	}

	return true;
}

/// 加载地图配置
bool CMapConfigManager::loadMapConfig(const char* pMapListFile)
{
	if (NULL == pMapListFile)
	{
		LOG_ERROR("load map list file failure, file path is null!");
		return false;
	}

	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pMapListFile))
	{
		LOG_ERROR("load map list file failure, %s", pMapListFile);
		return false;
	}

	XMLElement* pRootElement = tDocument.RootElement();
	if (NULL == pRootElement)
	{
		LOG_ERROR("map list file has no root element, %s", pMapListFile);
		return false;
	}

	XMLElement* pMapElement = pRootElement->FirstChildElement("Map");
	for (; NULL != pMapElement; pMapElement = pRootElement->NextSiblingElement("Map"))
	{
		unsigned short nMapID = pMapElement->IntAttribute("id");
		if (nMapID > 0 && nMapID < MAX_MAP_ID)
		{
			mMapConfig[nMapID] = new CMapConfig;
			if (NULL == mMapConfig[nMapID])
			{
				LOG_ERROR("new CMapConfig failure: %d", nMapID);
				return false;
			}

			bool bResult = mMapConfig[nMapID]->loadMapConfig(nMapID);
			if (!bResult)
			{
				return false;
			}

			bResult = mMapConfig[nMapID]->loadMapNPCConfig(nMapID);
			if (!bResult)
			{
				return false;
			}
		}
		else
		{
			LOG_ERROR("map id is invalid, %d", nMapID);
			return false;
		}
	}

	return true;
}

int CMapConfigManager::getMapPortalPos(unsigned short nSrcMapID, unsigned short nDesMapID, CMythPoint& rPortalPos)
{
	CMapConfig* pMapConfig = getMapConfig(nDesMapID);
	if (NULL == pMapConfig)
	{
		return -1;
	}

	return pMapConfig->getPortalPos(nSrcMapID, rPortalPos);
}