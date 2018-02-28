#include "rankmodule.h"
#include "timemanager.h"
#include "entityplayer.h"
#include "rankmodule.hxx.pb.h"
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

}

void CRankModule::sendGetRankInfoResponse()
{

}

// ������ҵ����а���ʱ��ô���ɣ������Ҫ������˵
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
		// ����������а��ֵ�����а�ʱ�䶼��ȵ�������ֶ�����ǰ�����а��ֵ��1
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
