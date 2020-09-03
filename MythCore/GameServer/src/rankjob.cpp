#include "rankjob.h"
#include "internalmsgpool.h"
#include "scenejob.h"
void CRankJob::doing(int uParam)
{
	int nCount = 0;
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// ���
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

	// ���scene job�Ѿ��˳�����ˣ���ʾ��Ҫ��������ݻ����������
	if (CSceneJob::Inst()->getExited())
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// ���û��������
		if (NULL == pIMMsg)
		{
			setExited(true);
		}
	}
}


// ������ҵ����а���ʱ��ô���ɣ������Ҫ������˵
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


// �����ҵ�����
int CRankJob::getRoleRank(EmRankType eType, int nRoleID)
{
	if (eType <= emRankType_None || eType >= emRankTypeMax)
	{
		return -1;
	}

	return mRankList[eType].getRankIndexByKey(nRoleID);
}
