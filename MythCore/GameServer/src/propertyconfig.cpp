#include "propertyconfig.h"
#include "tinyxml2.h"
#include "locallogjob.h"
bool CRechargeConfig::LoadRechargeConfig(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		LOG_ERROR("load recharge config pFilePath is null");
		return false;
	}

	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pFilePath))
	{
		// 出错，无法加载xml文件
		LOG_ERROR("load recharge config failure, %s", pFilePath);
		return false;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// 出错，连Root节点都没有
		LOG_ERROR("recharge config file has no root element, %s", pFilePath);
		return false;
	}

	mCheckChannelName = pRoot->BoolAttribute("CheckChannelName");
	XMLElement* pItemElem = pRoot->FirstChildElement("Item");
	for (; NULL != pItemElem; pItemElem = pItemElem->NextSiblingElement("Item"))
	{
		CRechargeGoods tGoods;
		tGoods.mGoodsID = pItemElem->Attribute("GoodsID");
		tGoods.mTitle = pItemElem->Attribute("Title");
		tGoods.mType = (byte)pItemElem->IntAttribute("Type");
		tGoods.mGameCoin = pItemElem->IntAttribute("GameCoin");
		tGoods.mCoinNum = pItemElem->DoubleAttribute("CoinNum") * 100 + 0.5;
		tGoods.mCoinType = pItemElem->Attribute("CoinType");
		tGoods.mVIPExp = pItemElem->IntAttribute("VIPExp");
		tGoods.mChannelName = pItemElem->Attribute("ChannelName");
		tGoods.mActivityCoin = pItemElem->IntAttribute("ActivityCoin");

		mGoodsList.push_back(tGoods);
	}

	return true;
}