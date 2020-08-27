#include "timeunit.h"
#include "scenejob.h"
#include "propertymodule.hxx.pb.h"
#include "locallogjob.h"
#include "GameLogicConfig.h"
#include "propertymodule.h"
#include "timemanager.h"
/// �����������
void CTimeUnit::onHeartBeatRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);

	// ������������ߣ����ߵ��Ե�ʱ��ǰ�˷������������ۼ���TCP Server��Game Server��ͨ�Źܵ��У�
	// ���ܻᵼ����ұ��ߵ�����
	time_t tNowTime = CTimeManager::Inst()->getCurrTime();

	CHeartBeatResponse tResponse;
	tResponse.set_servertime((uint)tNowTime);
	// �Ƿ����������
	if (CGameLogicConfig::Inst()->mHeartBeatCheck)
	{
		CGameLogicConfig* pLogicConfig = CGameLogicConfig::Inst();
		if (tNowTime - mHeartBeatTime < pLogicConfig->mHeartBeatTime)
		{
			++ mHeartBeatErrCount;
		}
		mHeartBeatTime = tNowTime;
		if (mHeartBeatErrCount >= pLogicConfig->mHeartBeatInvalidMax)
		{
			// �����������
			CPropertyModule::Inst()->playerLeaveGame(&mPlayer, EmLeaveReason_HeartBeatQuick);
			return;
		}
		// ����ﵽ���߳���һ������������ڣ�����
		if (tNowTime - mHeartBeatCheckTime >= pLogicConfig->mHeartBeatCheckTime)
		{
			mHeartBeatErrCount = 0;
			mHeartBeatCheckTime = tNowTime;
		}
	}

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_HEART_BEAT, &tResponse);
}
