#include "rankjob.h"
#include "internalmsgpool.h"
#include "scenejob.h"
#include "jobmanager.h"
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
			case IM_REQUEST_UPDATE_RANK:
			{
				onIMUpdateRankRequest(pIMMsg);
				break;
			}
			case IM_REQUEST_GET_RANK_INFO:
			{
				onIMGetRankInfoRequest(pIMMsg);
				break;
			}
		}

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
		++nCount;
		if (nCount > 100)
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

void CRankJob::onIMUpdateRankRequest(CInternalMsg* pIMMsg)
{
	if (NULL == pIMMsg)
	{
		return;
	}
	CIMUpdateRankRequest* pUpdateRankRequest = (CIMUpdateRankRequest*)pIMMsg;
	EmRankType eType = (EmRankType)(pUpdateRankRequest->mRankType);

	if (eType <= emRankType_None || eType >= emRankTypeMax)
	{
		return;
	}

	CRankValue tRankValue;
	tRankValue.mRankValue = pUpdateRankRequest->mValue;
	tRankValue.mRankTime = pUpdateRankRequest->mTime;
	tRankValue.mRankKey = pUpdateRankRequest->mRoleID;
	int nIndex = mRankList[eType].refreshRankValue(tRankValue);
	if (nIndex < MAX_RANK_SHOW_CACHE_NUM)
	{
		CIMUpdateRankResponse* pResponse = (CIMUpdateRankResponse*)CInternalMsgPool::Inst()->allocMsg(IM_RESPONSE_UPDATE_RANK);
		if (NULL != pResponse)
		{
			pResponse->mRoleID = pUpdateRankRequest->mRoleID;
			pResponse->mRoleObjID = pUpdateRankRequest->mRoleObjID;
			CJobManager::Inst()->pushTask(emJobTaskType_Scene, pResponse);
		}
	}
}

void CRankJob::onIMGetRankInfoRequest(CInternalMsg* pIMMsg)
{
	if (NULL == pIMMsg)
	{
		return;
	}
	CIMGetRankInfoRequest* pGetRankInfoRequest = (CIMGetRankInfoRequest*)pIMMsg;
	
	EmRankType eRankType = (EmRankType)(pGetRankInfoRequest->mRankType);
	int nStartPlace = pGetRankInfoRequest->mStartPlace;
	int nEndPlace = pGetRankInfoRequest->mEndPlace;
	uint nSelfRoleID = pGetRankInfoRequest->mSelfRoleID;
	int nSelfObjID = pGetRankInfoRequest->mSelfObjID;
	
	CIMGetRankInfoResponse* pResponse = (CIMGetRankInfoResponse*)CInternalMsgPool::Inst()->allocMsg(IM_RESPONSE_GET_RANK_INFO);
	if (NULL == pResponse)
	{
		return;
	}
	pResponse->mRankType = (byte)eRankType;
	int nCount = 0;
	for (int i = nStartPlace; i <= nEndPlace; ++ i)
	{
		CRankList::CRankValueType* pRankValueType = mRankList[eRankType].getRankValueByIndex(i);
		if (NULL == pRankValueType)
		{
			continue;
		}

		pResponse->mRankRoleID[nCount] = pRankValueType->mRankKey;
		pResponse->mRankValue[nCount]  = pRankValueType->mRankValue;
		++ nCount;
		if (nCount >= MAX_RANK_SHOW_NUM)
		{
			break;
		}
	}
	pResponse->mSelfRoleID = pGetRankInfoRequest->mSelfRoleID;
	pResponse->mSelfObjID = pGetRankInfoRequest->mSelfObjID;

	CRankList::CRankValueType* pRankValueType = mRankList[eRankType].getRankValueByKey(nSelfRoleID);
	if (NULL != pRankValueType)
	{
		pResponse->mSelfRankPlace = pRankValueType->mRankIndex;
		pResponse->mSelfRankValue = pRankValueType->mRankValue;
	}
	else
	{
		pResponse->mSelfRankPlace = -1;
		pResponse->mSelfRankValue = 0;
	}

	CJobManager::Inst()->pushTask(emJobTaskType_Scene, pResponse);
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

	mRankList[eType].refreshRankValue(tRankValue);
}

// 更新玩家的排行榜
void CRankJob::updateRoleRank(EmRankType eType, CRankValue& rRankValue)
{
	if (eType <= emRankType_None || eType >= emRankTypeMax)
	{
		return;
	}
	mRankList[eType].refreshRankValue(rRankValue);
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
