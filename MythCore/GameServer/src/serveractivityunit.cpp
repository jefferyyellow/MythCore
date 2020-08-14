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
	// �Ȱ��ϴ����ϵ���������ɾ�,�������úõ�ǰ��Ч���ʱ��
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

		// ��ջ��ʼʱ��
		mActTime[i] = 0;
		// �����Ƿ����Ͳ�����
		if (!pServerActivity->getAvail())
		{
			continue;
		}

		// ����������Ч
		mActTime[i] = pServerActivity->getStartTime();
	};
}