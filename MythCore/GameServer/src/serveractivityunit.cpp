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
	// �Ȱ��ϴ����ϵ���������ɾ�,�������úõ�ǰ��Ч���ʱ��
	for (int i = 0; i < MAX_SERVER_ACT_NUM; ++ i)
	{
		CServerActivity* pServerActivity = CServerActModule::Inst()->getServerActivity(i);
		if (NULL == pServerActivity)
		{
			continue;
		}
		// ���û���������
		if (mActTime[i] == 0)
		{
			if (!pServerActivity->getAvail())
			{
				continue;
			}
		}
		else
		{
			// ͬһ��������ڻ�ڼ���
			if (mActTime[i] == pServerActivity->getStartTime() && pServerActivity->getAvail())
			{
				continue;
			}
			// �Ѿ�����һ��
			lua_tinker::call<int>(L, "ServerActivity_ClearPlayerData", pServerActivity->getType(), pServerActivity->getID(), &mPlayer);

			// �����Ѿ����ǿ���״̬�����콱״̬��
			if (!pServerActivity->getAvail())
			{
				// ��ջ��ʼʱ��
				mActTime[i] = 0;
				continue;
			}
		}
		// ����������Ч
		mActTime[i] = pServerActivity->getStartTime();
	};
}