#include "serveractivity.h"
#include "errcode.h"
#include "serveractivityunit.h"
#include "entityplayer.h"
/// 加载配置文件
int CPhaseActivity::loadActivity(XMLElement* pActivityElem)
{
	mCondPrizeList.clear();
	XMLElement* pCondElem = pActivityElem->FirstChildElement("Cond");
	for (; NULL != pCondElem; pCondElem = pCondElem->NextSiblingElement("Cond"))
	{
		CCondPrizeData tPrizeData;
		tPrizeData.mCondNum = pCondElem->IntAttribute("Num");

		int nPrizeNum = 0;
		XMLElement* pPrizeElem = pCondElem->FirstChildElement("Prize");
		for (; NULL != pPrizeElem; pPrizeElem = pPrizeElem->NextSiblingElement("Prize"))
		{
			tPrizeData.mPrize[nPrizeNum].mItemID = pPrizeElem->IntAttribute("ItemID");
			tPrizeData.mPrize[nPrizeNum].mItemNum = pPrizeElem->IntAttribute("ItemNum");
			++nPrizeNum;
		}

		mCondPrizeList.push_back(tPrizeData);
	}
	return SUCCESS;
}

/// 得到配置文件的名字
const char* CPhaseActivity::getConfigFileName()
{
	return "phase_activity.xml";
}

/// 活动开启
void CPhaseActivity::start()
{

}

/// 活动结束
void CPhaseActivity::end()
{

}

/// 刷新玩家数据
void CPhaseActivity::refreshPlayerData(CEntityPlayer* pPlayer, int nParam)
{
	if (NULL == pPlayer)
	{
		return;
	}
	CServerActivityUnit& rActivityUnit = pPlayer->getServerActUnit();
	int nUniqueID = MAKE_ACT_UNIQUE_ID(mType, mSubType, mID);
	CServerActData*	pActData = rActivityUnit.getServerActData(nUniqueID);
	if (NULL == pActData)
	{
		return;
	}

	pActData->mParam1 += nParam;
}

/// 清空玩家数据
void CPhaseActivity::clearPlayerData(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	CServerActivityUnit& rActivityUnit = pPlayer->getServerActUnit();
	int nUniqueID = MAKE_ACT_UNIQUE_ID(mType, mSubType, mID);
	rActivityUnit.removeServerActData(nUniqueID);
}

void CPhaseActivity::getActivityPrize(CEntityPlayer* pPlayer, int nParam)
{
	if (NULL == pPlayer)
	{
		return;
	}

	CServerActivityUnit& rActivityUnit = pPlayer->getServerActUnit();
	int nUniqueID = MAKE_ACT_UNIQUE_ID(mType, mSubType, mID);
	CServerActData*	pActData = rActivityUnit.getServerActData(nUniqueID);
	if (NULL == pActData)
	{
		return;
	}

	if (nParam >= (int)mCondPrizeList.size())
	{
		return;
	}
	CCondPrizeData& rCondPrizeData = mCondPrizeList[nParam];
	if (pActData->getParam1() < rCondPrizeData.mCondNum)
	{
		return;
	}

	int nParam2 = pActData->mParam2;
	// 已经领取
	if (0 != (nParam2 & (1 << nParam)))
	{
		return;
	}

	int nItemID[ACTIVITY_PRIZE_NUM] = { 0 };
	int nItemNum[ACTIVITY_PRIZE_NUM] = { 0 };
	int nCount = 0;
	for (int i = 0; i < ACTIVITY_PRIZE_NUM; ++ i)
	{
		if (0 == rCondPrizeData.mPrize[i].mItemID)
		{
			break;
		}
		nItemID[nCount] = rCondPrizeData.mPrize[i].mItemID;
		nItemNum[nCount] = rCondPrizeData.mPrize[i].mItemNum;
		++nCount;
	}
	if (!pPlayer->getItemUnit().checkItemSpace(nItemID, nItemNum, nCount))
	{
		return;
	}

	pPlayer->getItemUnit().insertItem(nItemID, nItemNum, nCount);
	pActData->mParam2 |= (1 << nParam);
}