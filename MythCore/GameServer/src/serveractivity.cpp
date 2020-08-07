#include "serveractivity.h"
#include "errcode.h"
#include "serveractivityunit.h"
#include "entityplayer.h"

/// 加载配置文件
int CPhasePrizeActivity::loadActivity(XMLElement* pActivityElem)
{
	if (NULL == pActivityElem)
	{
		return ERR_UNKNOWN;
	}
	mCondPrizeList.clear();
	XMLElement* pCondElem = pActivityElem->FirstChildElement("Cond");
	for (; NULL != pCondElem; pCondElem = pCondElem->NextSiblingElement("Cond"))
	{
		CCondPrizeData tCondPrizeData;
		tCondPrizeData.mCondNum = pCondElem->IntAttribute("Num");

		XMLElement* pPrizeElem = pCondElem->FirstChildElement("Prize");
		for (; NULL != pPrizeElem; pPrizeElem = pPrizeElem->NextSiblingElement("Prize"))
		{
			CServerActPrize tPrize;
			tPrize.mItemID = pPrizeElem->IntAttribute("ItemID");
			tPrize.mItemNum = pPrizeElem->IntAttribute("ItemNum");
			tCondPrizeData.mPrizeList.push_back(tPrize);
		}

		mCondPrizeList.push_back(tCondPrizeData);
	}
	return SUCCESS;
}

/// 加载配置文件
int CCumulativeRecharge::loadActivity(XMLElement* pActivityElem)
{
	CPhasePrizeActivity::loadActivity(pActivityElem);
	return SUCCESS;
}


/// 得到配置文件的名字
const char* CCumulativeRecharge::getConfigFileName()
{
	return "cumulative_recharge.xml";
}

/// 清理玩家数据
void CCumulativeRecharge::clearPlayerData(CEntityPlayer& rPlayer)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	// 清除掉充值
	rServerActUnit.setActData(emActData_CumRechargeValue, 0);
	// 清除掉领奖记录
	rServerActUnit.setActData(emActData_CumRechargePrize, 0);
}

/// 刷新活动进度
void CCumulativeRecharge::refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	int nValue = rServerActUnit.getActData(emActData_CumRechargeValue) + nParam1;
	rServerActUnit.setActData(emActData_CumRechargeValue, nValue);
}

/// 得到活动奖励
void CCumulativeRecharge::getActivityPrize(CEntityPlayer& rPlayer, int nIndex)
{
	CCondPrizeData* pCondPrizeData = getCondPrizeData(nIndex);
	if (NULL == pCondPrizeData)
	{
		return;
	}

	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	// 已经领奖
	if (rServerActUnit.getActDataBit(emActData_CumRechargePrize, nIndex))
	{
		return;
	}

	// 没有达到要求
	if (rServerActUnit.getActData(emActData_CumRechargeValue) < pCondPrizeData->mCondNum)
	{
		return;
	}

	int nItemID[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nCount = 0;
	for (uint i = 0; i < pCondPrizeData->mPrizeList.size(); ++i)
	{
		nItemID[nCount] = pCondPrizeData->mPrizeList[i].mItemID;
		nItemNum[nCount] = pCondPrizeData->mPrizeList[i].mItemNum;
		++nCount;
	}
	if (!rPlayer.getItemUnit().checkItemSpace(nItemID, nItemNum, nCount))
	{
		return;
	}
	rPlayer.getItemUnit().insertItem(nItemID, nItemNum, nCount);
	rServerActUnit.setActDataBit(emActData_CumRechargePrize, nIndex);
}


/// 加载配置文件
int CCumulativeConsume::loadActivity(XMLElement* pActivityElem)
{
	CPhasePrizeActivity::loadActivity(pActivityElem);
	return SUCCESS;
}


/// 得到配置文件的名字
const char* CCumulativeConsume::getConfigFileName()
{
	return "cumulative_consume.xml";
}

/// 清理玩家数据
void CCumulativeConsume::clearPlayerData(CEntityPlayer& rPlayer)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	// 清除掉充值
	rServerActUnit.setActData(emActData_CumConsumeValue, 0);
	// 清除掉领奖记录
	rServerActUnit.setActData(emActData_CumConsumePrize, 0);
}

/// 刷新活动进度
void CCumulativeConsume::refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	int nValue = rServerActUnit.getActData(emActData_CumConsumeValue) + nParam1;
	rServerActUnit.setActData(emActData_CumConsumeValue, nValue);

}

/// 得到活动奖励
void CCumulativeConsume::getActivityPrize(CEntityPlayer& rPlayer, int nIndex)
{
	CCondPrizeData* pCondPrizeData = getCondPrizeData(nIndex);
	if (NULL == pCondPrizeData)
	{
		return;
	}

	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	// 已经领奖
	if (rServerActUnit.getActDataBit(emActData_CumConsumePrize, nIndex))
	{
		return;
	}

	// 没有达到要求
	if (rServerActUnit.getActData(emActData_CumConsumeValue) < pCondPrizeData->mCondNum)
	{
		return;
	}

	int nItemID[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nCount = 0;
	for (uint i = 0; i < pCondPrizeData->mPrizeList.size(); ++i)
	{
		nItemID[nCount] = pCondPrizeData->mPrizeList[i].mItemID;
		nItemNum[nCount] = pCondPrizeData->mPrizeList[i].mItemNum;
		++nCount;
	}
	if (!rPlayer.getItemUnit().checkItemSpace(nItemID, nItemNum, nCount))
	{
		return;
	}
	rPlayer.getItemUnit().insertItem(nItemID, nItemNum, nCount);
	rServerActUnit.setActDataBit(emActData_CumConsumePrize, nIndex);
}

/// 加载配置文件
int CRankActivity::loadActivity(XMLElement* pActivityElem)
{
	if (NULL == pActivityElem)
	{
		return ERR_UNKNOWN;
	}
	mRankPrizeList.clear();
	XMLElement* pRankElem = pActivityElem->FirstChildElement("Rank");
	for (; NULL != pRankElem; pRankElem = pRankElem->NextSiblingElement("Rank"))
	{
		CRankPrizeData tPrizeData;
		tPrizeData.mMinRank = pRankElem->IntAttribute("Min");
		tPrizeData.mMaxRank = pRankElem->IntAttribute("Max");
		XMLElement* pPrizeElem = pRankElem->FirstChildElement("Prize");
		for (; NULL != pPrizeElem; pPrizeElem = pPrizeElem->NextSiblingElement("Prize"))
		{
			CServerActPrize tPrize;
			tPrize.mItemID = pPrizeElem->IntAttribute("ItemID");
			tPrize.mItemNum = pPrizeElem->IntAttribute("ItemNum");
			tPrizeData.mPrizeList.push_back(tPrize);
		}

		mRankPrizeList.push_back(tPrizeData);
	}
	return SUCCESS;
}

/// 得到配置文件的名字
const char* CRankActivity::getConfigFileName()
{
	return "rank_activity.xml";
}

/// 清理玩家数据
void CRankActivity::clearPlayerData(CEntityPlayer& rPlayer)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	switch (mSubType)
	{
		case emRankActSubType_Level:
		{
			rServerActUnit.clearBit(emActBit_LevelRankPrize);
			break;
		}
		case emRankActSubType_Power:
		{
			rServerActUnit.clearBit(emActBit_PowerRankPrize);
			break;
		}
		default:
		{
			break;
		}
	}
}

/// 得到活动奖励
void CRankActivity::getActivityPrize(CEntityPlayer& rPlayer)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	CRankPrizeData* pRankPrizeData = NULL;
	int nRank = 0;
	switch (mSubType)
	{
		case emRankActSubType_Level:
		{
			// 已经领奖了
			if (rServerActUnit.getActBit(emActBit_LevelRankPrize))
			{
				return;
			}
			
			break;
		}
		case emRankActSubType_Power:
		{
			// 已经领奖了
			if (rServerActUnit.getActBit(emActBit_PowerRankPrize))
			{
				return;
			}
			break;
		}
		default:
		{
			return;
		}
	}

	for (uint i = 0; i < mRankPrizeList.size(); ++ i)
	{
		if (nRank >= mRankPrizeList[i].mMinRank && nRank <= mRankPrizeList[i].mMaxRank)
		{
			pRankPrizeData = &mRankPrizeList[i];
			break;
		}
	}

	// 不在有奖励的人里面
	if (NULL == pRankPrizeData)
	{
		return;
	}

	int nItemID[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nCount = 0;
	for (uint i = 0; i < pRankPrizeData->mPrizeList.size(); ++i)
	{
		nItemID[nCount] = pRankPrizeData->mPrizeList[i].mItemID;
		nItemNum[nCount] = pRankPrizeData->mPrizeList[i].mItemNum;
		++nCount;
	}
	if (!rPlayer.getItemUnit().checkItemSpace(nItemID, nItemNum, nCount))
	{
		return;
	}
	rPlayer.getItemUnit().insertItem(nItemID, nItemNum, nCount);

	// 设置已经领奖的标志
	switch (mSubType)
	{
		case emRankActSubType_Level:
		{
			rServerActUnit.setActBit(emActBit_LevelRankPrize);
			break;
		}
		case emRankActSubType_Power:
		{
			rServerActUnit.setActBit(emActBit_PowerRankPrize);
			break;
		}
		default:
		{
			return;
		}
	}
}