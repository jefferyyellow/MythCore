#include "rankmodule.h"
#include "timemanager.h"
#include "entityplayer.h"
#include "rankmodule.hxx.pb.h"
#include "scenejob.h"
/*
	for (int i = 0; i < 1000; ++ i)
	{
	CRankValue tRankValue;
	tRankValue.mRankValue = RAND(1000);
	tRankValue.mRankKey = i;
	tRankValue.mRankTime = i;
	mRankList[eType].refreshRankValue(tRankValue);
	}

	vector<int> tv;
	char acBuffer[MAX_PATH] = {0};
	for (int i = 0; i < mRankList[eType].GetNum(); ++i)
	{
	CRankList::CRankValueType* pRankValueType = mRankList[eType].getRankValueByIndex(i);
	snprintf(acBuffer, sizeof(acBuffer) - 1, "%d*********%d\n", pRankValueType->mRankValue, pRankValueType->mRankTime);
	OutputDebugString(acBuffer);
	}
*/
CRankModule::CRankModule()
{
	
}

CRankModule::~CRankModule()
{
	
}

/// 启动服务器
void CRankModule::onLaunchServer()
{

}

/// 启动完成检查
bool CRankModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CRankModule::onLaunchComplete()
{

}

/// 退出服务器
void CRankModule::onExitServer()
{

}

/// 新一天到来
void CRankModule::onNewDayCome()
{

}

/// 新一周到来
void CRankModule::onNewWeekCome()
{

}

/// 建立实体
void CRankModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
void CRankModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CRankModule::onTimer(unsigned int nTickOffset)
{

}

void CRankModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_GET_RANK_INFO:
		{
			onGetRankInfoRequest(pPlayer, pMessage);
			break;
		}
		default:
			break;
	}
}

void CRankModule::onGetRankInfoRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pPlayer && NULL != pMessage, return);
	CGetRankInfoRequest* pGetRankInfoRequest = static_cast<CGetRankInfoRequest*>(pMessage);
	MYTH_ASSERT(NULL != pGetRankInfoRequest, return);

	sendGetRankInfoResponse(pPlayer, (EmRankType)pGetRankInfoRequest->ranktype());
}

void CRankModule::sendGetRankInfoResponse(CEntityPlayer* pPlayer, EmRankType eType)
{
	CGetRankInfoResponse tResponse;
	tResponse.set_ranktype(eType);

	int nRoldId[MAX_RANK_SHOW_NUM] = {0};
	int nRankValue[MAX_RANK_SHOW_NUM] = {0};

	for (int i = 0; i < MAX_RANK_SHOW_NUM; ++ i)
	{
		CRankList::CRankValueType* pRankValueType = mRankList[eType].getRankValueByIndex(i);
		if (NULL == pRankValueType)
		{
			continue;
		}

		PBRankRoleInfo* pRankRoleInfo = tResponse.add_roleinfo();
		if (NULL == pRankRoleInfo)
		{
			continue;
		}
		pRankRoleInfo->set_roleid(pRankValueType->mRankKey);
		pRankRoleInfo->set_rankvalue(pRankValueType->mRankValue);
	}

	CSceneJob::Inst()->send2Player(pPlayer, ID_C2S_REQUEST_GET_RANK_INFO, &tResponse);
}

// 更新玩家的排行榜，暂时这么做吧，如果需要缓存再说
void CRankModule::updateRoleRank(EmRankType eType, CEntityPlayer* pPlayer, int nValue)
{
	if (eType <= emRankType_None || eType >= emRankTypeMax)
	{
		return;
	}

	CRankValue tRankValue;
	tRankValue.mRankValue = nValue;
	tRankValue.mRankTime = (int)CTimeManager::Inst()->getCurrTime();
	tRankValue.mRankKey = pPlayer->getRoleID();

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
int CRankModule::getRoleRank(EmRankType eType, int nRoleID)
{
	if (eType <= emRankType_None || eType >= emRankTypeMax)
	{
		return -1;
	}

	return mRankList[eType].getRankIndexByKey(nRoleID);
}
