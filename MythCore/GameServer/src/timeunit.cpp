#include "timeunit.h"
#include "scenejob.h"
#include "propertymodule.hxx.pb.h"
#include "locallogjob.h"
#include "GameLogicConfig.h"
#include "propertymodule.h"
/// 玩家心跳请求
void CTimeUnit::onHeartBeatRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);

	time_t tNowTime = CTimeManager::Inst()->getCurrTime();
	CHeartBeatResponse tResponse;
	tResponse.set_servertime((uint)tNowTime);
	// 是否做心跳检测
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
			// 将玩家踢下线
			CPropertyModule::Inst()->playerLeaveGame(&mPlayer);
			return;
		}
		// 如果达到或者超过一个心跳检查周期，重置
		if (tNowTime - mHeartBeatCheckTime >= pLogicConfig->mHeartBeatCheckTime)
		{
			mHeartBeatErrCount = 0;
			mHeartBeatCheckTime = tNowTime;
		}
	}

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_HEART_BEAT, &tResponse);
}
