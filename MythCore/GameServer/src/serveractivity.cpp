#include "serveractivity.h"
#include "errcode.h"
#include "serveractivityunit.h"
#include "entityplayer.h"

/// ���������ļ�
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

/// ���������ļ�
int CCumulativeRecharge::loadActivity(XMLElement* pActivityElem)
{
	CPhasePrizeActivity::loadActivity(pActivityElem);
	return SUCCESS;
}


/// �õ������ļ�������
const char* CCumulativeRecharge::getConfigFileName()
{
	return "cumulative_recharge.xml";
}

/// �����������
void CCumulativeRecharge::clearPlayerData(CEntityPlayer& rPlayer)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	// �������ֵ
	rServerActUnit.setActData(emActData_CumRechargeValue, 0);
	// ������콱��¼
	rServerActUnit.setActData(emActData_CumRechargePrize, 0);
}

/// ˢ�»����
void CCumulativeRecharge::refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	int nValue = rServerActUnit.getActData(emActData_CumRechargeValue) + nParam1;
	rServerActUnit.setActData(emActData_CumRechargeValue, nValue);
}

/// �õ������
void CCumulativeRecharge::getActivityPrize(CEntityPlayer& rPlayer, int nIndex)
{
	CCondPrizeData* pCondPrizeData = getCondPrizeData(nIndex);
	if (NULL == pCondPrizeData)
	{
		return;
	}

	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	// �Ѿ��콱
	if (rServerActUnit.getActDataBit(emActData_CumRechargePrize, nIndex))
	{
		return;
	}

	// û�дﵽҪ��
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


/// ���������ļ�
int CCumulativeConsume::loadActivity(XMLElement* pActivityElem)
{
	CPhasePrizeActivity::loadActivity(pActivityElem);
	return SUCCESS;
}


/// �õ������ļ�������
const char* CCumulativeConsume::getConfigFileName()
{
	return "cumulative_consume.xml";
}

/// �����������
void CCumulativeConsume::clearPlayerData(CEntityPlayer& rPlayer)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	// �������ֵ
	rServerActUnit.setActData(emActData_CumConsumeValue, 0);
	// ������콱��¼
	rServerActUnit.setActData(emActData_CumConsumePrize, 0);
}

/// ˢ�»����
void CCumulativeConsume::refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2)
{
	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	int nValue = rServerActUnit.getActData(emActData_CumConsumeValue) + nParam1;
	rServerActUnit.setActData(emActData_CumConsumeValue, nValue);

}

/// �õ������
void CCumulativeConsume::getActivityPrize(CEntityPlayer& rPlayer, int nIndex)
{
	CCondPrizeData* pCondPrizeData = getCondPrizeData(nIndex);
	if (NULL == pCondPrizeData)
	{
		return;
	}

	CServerActUnit& rServerActUnit = rPlayer.getServerActUnit();
	// �Ѿ��콱
	if (rServerActUnit.getActDataBit(emActData_CumConsumePrize, nIndex))
	{
		return;
	}

	// û�дﵽҪ��
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

/// ���������ļ�
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

/// �õ������ļ�������
const char* CRankActivity::getConfigFileName()
{
	return "rank_activity.xml";
}

/// �����������
void CRankActivity::clearPlayerData(CEntityPlayer& rPlayer)
{

}