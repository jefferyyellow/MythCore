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
		init();
	}
	~CTaskModule()
	{
	}

	int init()
	{
        memset(mTaskConfig, NULL, sizeof(mTaskConfig));
	}

	/// 启动服务器
	virtual void onLaunchServer();
	/// 启动完成检查
	virtual bool onCheckLaunch();
	/// 服务器启动成功
	virtual void onLaunchComplete();
	/// 退出服务器
	virtual void onExitServer();
	/// 新一天到来
	virtual void onNewDayCome();
	/// 新一周到来
	virtual void onNewWeekCome();
	/// 建立实体
	virtual void onCreatePlayer(CEntityPlayer* pPlayer);
	/// 销毁实体
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer);
	/// 时间函数
	virtual void onTimer(unsigned int nTickOffset);

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