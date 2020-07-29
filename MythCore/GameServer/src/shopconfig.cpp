#include "shopconfig.h"
#include "locallogjob.h"
#include "itemmodule.hxx.pb.h"
void CShopGoods::createToPB(PBShopGoods* pbGoods)
{
	if (NULL == pbGoods)
	{
		return;
	}

	pbGoods->set_goodsid(mGoodsID);
	pbGoods->set_goodsnum(mGoodsNum);
	pbGoods->set_consumeid(mConsumeID);
	pbGoods->set_consumenum(mConsumeNum);
	pbGoods->set_tabindex(mTabIndex);
}

bool CShopConfig::loadShopConfig(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		LOG_ERROR("load shop config failure, FilePath is null");
		return false;
	}

	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pFilePath))
	{
		// 出错，无法加载xml文件
		LOG_ERROR("load shop config failur, %s", pFilePath);
		return false;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// 出错，连Root节点都没有
		LOG_ERROR("shop config has no root element, %s", pFilePath);
		return false;
	}

	XMLElement* pLevelDataElem = pRoot->FirstChildElement("LevelData");
	for (; NULL != pLevelDataElem; pLevelDataElem = pLevelDataElem->NextSiblingElement("LevelData"))
	{
		CShopLevelData tLevelData;

		tLevelData.mMinLevel = pLevelDataElem->IntAttribute("MinLevel");
		tLevelData.mMaxLevel = pLevelDataElem->IntAttribute("MaxLevel");

		XMLElement* pGoodsElem = pLevelDataElem->FirstChildElement("Goods");
		for (; NULL != pGoodsElem; pGoodsElem = pGoodsElem->NextSiblingElement("Goods"))
		{
			CShopGoods tGoods;
			tGoods.mGoodsID = pGoodsElem->IntAttribute("ID");
			tGoods.mGoodsNum = pGoodsElem->IntAttribute("Num");
			tGoods.mConsumeID = pGoodsElem->IntAttribute("ConsumeID");
			tGoods.mConsumeNum = pGoodsElem->IntAttribute("ConsumeNum");
			tGoods.mTabIndex = pGoodsElem->IntAttribute("TabIndex");

			tLevelData.mGoodsList.push_back(tGoods);
		}

		mLevelList.push_back(tLevelData);
	}

	return true;
}

CShopLevelData*	CShopConfig::GetLevelData(int nLevel)
{
	for (uint i = 0; i < mLevelList.size(); ++ i)
	{
		if (nLevel >= mLevelList[i].mMinLevel && nLevel <= mLevelList[i].mMaxLevel)
		{
			return &mLevelList[i];
		}
	}

	return NULL;
}
