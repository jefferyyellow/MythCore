#include "rankmodule.h"
#include "entityplayer.h"
#include "rankmodule.hxx.pb.h"
#include "scenejob.h"
#include "timemanager.h"
#include "internalmsgpool.h"
#include "jobmanager.h"
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

/// 加载配置文件
void CRankModule::onLoadConfig()
{

}


void CRankModule::onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_GET_RANK_INFO:
		{
			onGetRankInfoRequest(rPlayer, pMessage);
			break;
		}
		default:
			break;
	}
}

void CRankModule::onGetRankInfoRequest(CEntityPlayer& rPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CGetRankInfoRequest* pGetRankInfoRequest = static_cast<CGetRankInfoRequest*>(pMessage);
	MYTH_ASSERT(NULL != pGetRankInfoRequest, return);

	CIMGetRankInfoRequest* pRequest = (CIMGetRankInfoRequest*)CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_GET_RANK_INFO);
	if (NULL == pRequest)
	{
		return;
	}

	pRequest->mRankType = pGetRankInfoRequest->ranktype();
	pRequest->mStartPlace = 0;
	pRequest->mEndPlace = MAX_RANK_SHOW_NUM - 1;
	pRequest->mSelfRoleID = rPlayer.getRoleID();
	pRequest->mSelfObjID = rPlayer.getObjID();
	CJobManager::Inst()->pushTask(emJobTaskType_Rank, pRequest);
}

void CRankModule::onIMGetRankInfoResponse(CInternalMsg* pIMMsg)
{
	if (NULL == pIMMsg)
	{
		return;
	}

	CIMGetRankInfoResponse* pIMResponse = (CIMGetRankInfoResponse*)pIMMsg;
	CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(pIMResponse->mSelfObjID);
	if (NULL == pPlayer)
	{
		return;
	}
	if (pPlayer->getRoleID() != pIMResponse->mSelfRoleID)
	{
		return;
	}

	CGetRankInfoResponse tResponse;
	tResponse.set_ranktype(pIMResponse->mRankType);

	for (int i = 0; i < pIMResponse->mCount; ++i)
	{
		PBRankRoleInfo* pRankRoleInfo = tResponse.add_roleinfo();
		if (NULL == pRankRoleInfo)
		{
			continue;
		}
		pRankRoleInfo->set_roleid(pIMResponse->mRankRoleID[i]);
		pRankRoleInfo->set_rankvalue(pIMResponse->mRankValue[i]);
	}

	tResponse.set_selfplace(pIMResponse->mSelfRankPlace);
	tResponse.set_selfrankvalue(pIMResponse->mSelfRankValue);

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_RESPONSE_GET_RANK_INFO, &tResponse);
}

// 更新玩家的排行榜
void CRankModule::updateRoleRank(EmRankType eType, uint nRoleID, int nValue)
{
	CIMUpdateRankRequest* pUpdateRankRequest = (CIMUpdateRankRequest*)CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_UPDATE_RANK);
	if (NULL == pUpdateRankRequest)
	{
		return;
	}

	pUpdateRankRequest->mRankType = eType;
	pUpdateRankRequest->mRoleID = nRoleID;
	pUpdateRankRequest->mValue = nValue;
	pUpdateRankRequest->mTime = CTimeManager::Inst()->getCurrTime();

	CJobManager::Inst()->pushTask(emJobTaskType_Rank, pUpdateRankRequest);
}

