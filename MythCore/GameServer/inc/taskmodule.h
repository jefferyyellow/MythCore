#ifndef __TASKMODULE_H__
#define __TASKMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "messagefactory.h"
#include "taskunit.h"
using namespace Myth;
class CEntityPlayer;
class CTaskConfig;
class CTaskModule : public CLogicModule, public CSingleton < CTaskModule >
{
	friend class CSingleton <CTaskModule> ;
private:
	CTaskModule()
	{
		memset(mTaskConfig, 0, sizeof(mTaskConfig));
	}
	~CTaskModule()
	{
	}

	/// 时间函数
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	// 加载所以的任务配置
	void	loadAllTaskConfig(const char* pPath);
	// 加载单个任务配置
	void	loadTaskConfig(const char* pFileName);

	CTaskConfig*			getTaskConfig(unsigned int nTaskID)
	{
		if (nTaskID >= MAX_TASK_ID)
		{
			return NULL;
		}
		return mTaskConfig[nTaskID];
	}

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

private:
	CTaskConfig*			mTaskConfig[MAX_TASK_ID];	// 任务配置
};
#endif