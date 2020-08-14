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
void CServerActUnit::dailyRefresh()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
	// 先把上次身上的数据清理干净,并且设置好当前有效活动的时间
	for (int i = 0; i < MAX_SERVER_ACT_NUM; ++ i)
	{
		if (0 == mActTime[i])
		{
			continue;
		}

		CServerActivity* pServerActivity = CServerActModule::Inst()->getServerActivity(i);
		if (NULL == pServerActivity)
		{
			continue;
		}

		if (mActTime[i] == pServerActivity->getStartTime())
		{
			continue;
		}
		lua_tinker::call<int>(L, pServerActivity->getClearPlayerData(), 
			pServerActivity->getID(), &mPlayer);

		// 清空活动开始时间
		mActTime[i] = 0;
		// 如果活动非法，就不管了
		if (!pServerActivity->getAvail())
		{
			continue;
		}

		// 这个活动重新有效
		mActTime[i] = pServerActivity->getStartTime();
	};
}