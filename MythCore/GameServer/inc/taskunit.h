#ifndef __TASKUNIT_H__
#define __TASKUNIT_H__
#include "playersubunit.h"
#include "bit_set.h"
#include "messagefactory.h"
#include "taskconfig.h"
#define		MAX_PLAYER_TASK_NUM		20			// 最多的可接任务数目
#define		MAX_PLAYER_TASK_PARAM	4			// 最多的任务参数数目

enum EmTaskState
{
	emTaskState_None			= 0,	// 无类型
	emTaskState_Accept			= 1,	// 任务已接受
	emTaskState_Complete		= 2,	// 任务已完成
	emTaskState_Failure			= 3,	// 任务已失败
	emTaskStateMax
};

/// 玩家任务
class CPlayerTask
{
public:
	CPlayerTask()
	{
		init();
	}
	~CPlayerTask()
	{

	}
	void			init()
	{
		mTaskID = 0;
		mTaskStatus = 0;
		memset(mTaskParam, 0, sizeof(mTaskParam));
	}
public:
	// 任务ID
	short			getTaskID() const { return mTaskID; }
	void			setTaskID(short nValue) { mTaskID = nValue; }

	// 任务进度
	short			getTaskStatus() const { return mTaskStatus; }
	void			setTaskStatus(short nValue) { mTaskStatus = nValue; }

	int			getTaskParam(int nIndex)
	{
		if (nIndex < 0 || nIndex >= MAX_PLAYER_TASK_PARAM)
		{
			return 0;
		}
		return mTaskParam[nIndex];
	}

	void			setTaskParam(int nIndex, int nTaskParam)
	{
		if (nIndex < 0 || nIndex >= MAX_PLAYER_TASK_PARAM)
		{
			return;
		}

		mTaskParam[nIndex] = nTaskParam;
	}
private:
	/// 任务ID
	short			mTaskID;
	/// 任务进度
	short			mTaskStatus;
	/// 任务参数
	int				mTaskParam[MAX_PLAYER_TASK_PARAM];
};

class CEntityPlayer;
class CTaskUnit : public CPlayerSubUnit
{
	typedef vector<CPlayerTask> PLAYER_TASK_LIST;
public:
	CTaskUnit(CEntityPlayer& rPlayer)
		: CPlayerSubUnit(rPlayer)
	{
		mMaxCompleteTaskID = 0;
	}
	~CTaskUnit()
	{

	}

public:
	void	fireEvent(EmCompleteCondition eCondition, int nParam1, int nParam2);
	void	setTaskComplete(int nTaskID)
	{
		mCompleteTasks.setBit(nTaskID);
		if (nTaskID > mMaxCompleteTaskID)
		{
			mMaxCompleteTaskID = nTaskID;
		}
	}

public:
	void onAcceptTaskRequest(Message* pMessage);
	void sendAcceptTaskResponse(int nResult, int nTaskID);

	void onSubmitTaskRequest(Message* pMessage);
	void sendSubmitTaskResponse(int nResult, int nTaskID);

public:
	short getMaxCompleteTaskID() const { return mMaxCompleteTaskID; }
	void setMaxCompleteTaskID(short nValue) { mMaxCompleteTaskID = nValue; }
	/// 检查任务是否能接受
	int	checkAcceptTask(int nTaskID);
	/// 任务接受后处理
	void afterAcceptTask(int nTaskID);
	/// 检查任务是否能提交
	int checkSubmitTask(int nTaskID, short& bRepeated);

	/// 根据ID得到玩家身上的任务
	CPlayerTask* getPlayerTask(int nTaskID);
	/// 删除玩家身上的任务
	void removeTask(int nTaskID);
private:
	/// 所有已经完成的任务
	Myth::CBitSet<MAX_TASK_ID>		mCompleteTasks;
	/// 最大的已经完成的任务
	short							mMaxCompleteTaskID;
	/// 已经接受的任务列表
	PLAYER_TASK_LIST				mTaskList;
};
#endif