#include "serveractivityunit.h"
#include "locallogjob.h"
#include "serveractmodule.h"
#include "serveractivity.h"
#include "scenejob.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"
#include "timemanager.h"
void CServerActUnit::dailyRefresh()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
	time_t tNowTime = CTimeManager::Inst()->getCurrTime();
	// 先把上次身上的数据清理干净,并且设置好当前有效活动的时间
	for (int i = 0; i < MAX_SERVER_ACT_NUM; ++ i)
	{
		CServerActivity* pServerActivity = CServerActModule::Inst()->getServerActivity(i);
		if (NULL == pServerActivity)
		{
			continue;
		}
		// 玩家没有相关数据
		if (mActTime[i] == 0)
		{
			if (!pServerActivity->getAvail())
			{
				continue;
			}
		}
		else
		{
			// 同一个活动并且在活动期间内
			if (mActTime[i] == pServerActivity->getStartTime() && pServerActivity->getAvail())
			{
				continue;
			}
			// 已经不是一个
			lua_tinker::call<int>(L, "ServerActivity_ClearPlayerData", pServerActivity->getType(), pServerActivity->getID(), &mPlayer);

			// 如果活动已经不是开启状态或者领奖状态了
			if (!pServerActivity->getAvail())
			{
				// 清空活动开始时间
				mActTime[i] = 0;
				continue;
			}
		}
		// 这个活动重新有效
		mActTime[i] = pServerActivity->getStartTime();
	};
}