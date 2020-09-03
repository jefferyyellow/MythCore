#include "rankjob.h"
#include "internalmsgpool.h"
#include "scenejob.h"
void CRankJob::doing(int uParam)
{
	int nCount = 0;
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}

		switch (pIMMsg->getMsgID())
		{
			
		}

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
		++nCount;
		if (nCount > 1000)
		{
			break;
		}
	}

	// 如果scene job已经退出完成了，表示需要保存的数据基本都完成了
	if (CSceneJob::Inst()->getExited())
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果没有任务了
		if (NULL == pIMMsg)
		{
			setExited(true);
		}
	}
}


// 更新玩家的排行榜，暂时这么做吧，如果需要缓存再说
void CRankJob::updateRoleRank(EmRankType eType, uint nRoleID, int nValue, time_t nTime)
{
	if (eType <= emRankType_None || eType >= emRankTypeMax)
	{
		return;
	}

	CRankValue tRankValue;
	tRankValue.mRankValue = nValue;
	tRankValue.mRankTime = nTime;
	tRankValue.mRankKey = nRoleID;

	int nIndex = mRankList[eType].refreshRankValue(tRankValue);
	if (nIndex >= 1)
	{
		// 如果出现排行榜的值和排行榜时间都相等的情况，手动将当前的排行榜的值加1
		CRankList::CRankValueType* pPreValueType = mRankList[eType].getRankValueByIndex(nIndex - 1);
		CRankList::CRankValueType* pValueType = mRankList[eType].getRankValueByIndex(nIndex);
		if (NULL != pValueType && NULL != pPreValueType
			&& pValueType->mRankValue == pPreValueType->mRankValue
			&& pValueType->mRankTime == pPreValueType->mRankTime)
		{
			pValueType->mRankTime += 1;
		}
	}
}


// 获得玩家的排名
int CRankJob::getRoleRank(EmRankType eType, int nRoleID)
{
	if (eType <= emRankType_None || eType >= emRankTypeMax)
	{
		return -1;
	}

	return mRankList[eType].getRankIndexByKey(nRoleID);
}
