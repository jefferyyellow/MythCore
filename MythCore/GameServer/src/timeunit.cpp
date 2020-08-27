#include "timeunit.h"
#include "scenejob.h"
#include "propertymodule.hxx.pb.h"
#include "locallogjob.h"
#include "GameLogicConfig.h"
#include "propertymodule.h"
#include "timemanager.h"
/// 玩家心跳请求
void CTimeUnit::onHeartBeatRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);

	// 如果服务器下线，或者调试的时候，前端发过来心跳包累计在TCP Server和Game Server的通信管道中，
	// 可能会导致玩家被踢掉下线
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
			CPropertyModule::Inst()->playerLeaveGame(&mPlayer, EmLeaveReason_HeartBeatQuick);
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
