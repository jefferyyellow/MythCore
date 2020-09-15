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

/// ����������
void CRankModule::onLaunchServer()
{

}

/// ������ɼ��
bool CRankModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CRankModule::onLaunchComplete()
{

}

/// �˳�������
void CRankModule::onExitServer()
{

}

/// ��һ�쵽��
void CRankModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CRankModule::onNewWeekCome()
{

}

/// ����ʵ��
void CRankModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
void CRankModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CRankModule::onTimer(unsigned int nTickOffset)
{

}

/// ���������ļ�
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

void CRankModule::onIMUpdateRankResponse(CInternalMsg* pIMMsg)
{
	if (NULL == pIMMsg)
	{
		return;
	}
	CIMUpdateRankResponse* pResponse = (CIMUpdateRankResponse*)pIMMsg;

	CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(pResponse->mRoleObjID);
	// �Ѿ�������
	if (NULL == pPlayer)
	{
		return;
	}
	// �Ѿ���������
	if (pPlayer->getRoleID() != pResponse->mRoleID)
	{
		return;
	}

	addRoleInfo(*pPlayer);
}


// ������ҵ����а�
void CRankModule::updateRoleRank(EmRankType eType, CEntityPlayer& rPlayer, int nValue)
{
	CIMUpdateRankRequest* pUpdateRankRequest = (CIMUpdateRankRequest*)CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_UPDATE_RANK);
	if (NULL == pUpdateRankRequest)
	{
		return;
	}

	pUpdateRankRequest->mRankType = eType;
	pUpdateRankRequest->mRoleID = rPlayer.getRoleID();
	pUpdateRankRequest->mRoleObjID = rPlayer.getObjID();
	pUpdateRankRequest->mValue = nValue;
	pUpdateRankRequest->mTime = CTimeManager::Inst()->getCurrTime();

	CJobManager::Inst()->pushTask(emJobTaskType_Rank, pUpdateRankRequest);
}


// ����������а���Ϣ
void CRankModule::updateRoleInfo(CEntityPlayer& rPlayer)
{
	HASH_RANK_INFO::iterator it = mHashRankInfo.find(rPlayer.getRoleID());
	// ����
	if (it == mHashRankInfo.end())
	{
		return;
	}
	CPlayerRankInfo& rRankInfo = it->second;
	updateRoleInfo(rPlayer, rRankInfo);
}

/// ����������а���Ϣ
void CRankModule::updateRoleInfo(CEntityPlayer& rPlayer, CPlayerRankInfo& rRankInfo)
{
	rRankInfo.setName(rPlayer.getName());
	rRankInfo.setHeadID(rPlayer.getPropertyUnit().getHeadID());
}

/// ����������а���Ϣ
void CRankModule::addRoleInfo(CEntityPlayer& rPlayer)
{
	HASH_RANK_INFO::iterator it = mHashRankInfo.find(rPlayer.getRoleID());
	// ����
	if (it != mHashRankInfo.end())
	{
		CPlayerRankInfo& rRankInfo = it->second;
		updateRoleInfo(rPlayer, rRankInfo);
	}
	else
	{
		CPlayerRankInfo tRankInfo;
		updateRoleInfo(rPlayer, tRankInfo);
		mHashRankInfo.insert(HASH_RANK_INFO::value_type(rPlayer.getRoleID(), tRankInfo));
	}
}