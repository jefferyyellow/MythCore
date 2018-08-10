#ifndef __TASKUNIT_H__
#define __TASKUNIT_H__
#include "playersubunit.h"
#include "bit_set.h"
#include "messagefactory.h"
#include "taskconfig.h"
class PBTaskList;
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
        memset(mTaskParam, 0, sizeof(mTaskParam));
        memset(mCondType, 0, sizeof(mCondType));
	}

public:
	int				getParamByCondType(int nType)
	{
		for (int i = 0; i < MAX_COMPLETE_CONDITION; ++i)
		{
			if (mCondType[i] == nType)
			{
				return mTaskParam[i];
			}
		}

		return 0;
	}
public:
	/// autocode don't edit!!!
    short getTaskID(){ return mTaskID;}
    void setTaskID(short value){ mTaskID = value;}

    int getTaskParam(int nIndex)
    {
        if(nIndex < 0 || nIndex >= MAX_COMPLETE_CONDITION)
        {
            return 0;
        }
        return mTaskParam[nIndex];
    }
    void setTaskParam(int nIndex, int value)
    {
        if(nIndex < 0 || nIndex >= MAX_COMPLETE_CONDITION)
        {
            return;
        }
        mTaskParam[nIndex] = value;
    }

    int getCondType(int nIndex)
    {
        if(nIndex < 0 || nIndex >= MAX_COMPLETE_CONDITION)
        {
            return 0;
        }
        return mCondType[nIndex];
    }
    void setCondType(int nIndex, int value)
    {
        if(nIndex < 0 || nIndex >= MAX_COMPLETE_CONDITION)
        {
            return;
        }
        mCondType[nIndex] = value;
    }
	/// end autocode


private:
	/// 任务ID
	short			mTaskID;
	/// 任务参数
	int				mTaskParam[MAX_COMPLETE_CONDITION];
	/// 条件类型
	int				mCondType[MAX_COMPLETE_CONDITION];
};

class CEntityPlayer;
class CTaskUnit : public CPlayerSubUnit
{
	typedef vector<CPlayerTask> PLAYER_TASK_LIST;
public:
	CTaskUnit(CEntityPlayer& rPlayer)
		: CPlayerSubUnit(rPlayer)
	{
		init();
	}
	~CTaskUnit()
	{

	}
	void init()
	{
	}

public:
	void	setTaskComplete(int nTaskID)
	{
		mCompleteTasks.setBit(nTaskID);
	}

public:
	void onAcceptTaskRequest(Message* pMessage);
	void sendAcceptTaskResponse(int nResult, int nTaskID);

	void onSubmitTaskRequest(Message* pMessage);
	void sendSubmitTaskResponse(int nResult, int nTaskID);

	/// 处理放弃任务请求
	void onAbortTaskRequest(Message* pMessage);
	/// 发送放弃任务回应
	void sendAbortTaskResponse(int nResult, int nTaskID);
	/// 发送更新任务进度通知
	void sendUpdateTaskProcessNotify(int nTaskID, int nCondIndex, int nParam);

	// 检查是否可以接受任务
	int		checkAcceptTask(int nTaskID);
	// 检查是否满足接受任务条件
	int		checkAcceptCondition(EmAcceptCondition eCondition, int nParam1, int nParam2);
	// 检查是否可以关闭任务
	int		checkCloseTask(int nTaskID);
	// 检查是否满足关闭任务条件
	int		checkCloseCondition(CTaskCondition& rCondition);
	// 检查是否可以提交任务
	int		checkCommitTask(int nTaskID, int nItemIndex);
	// 检查是否满足提交任务条件
	int		checkCommitCondition(CPlayerTask* pPlayerTask, int nParamIndex, CTaskCondition& rCondition, int nItemIndex);
	// 扣除任务道具
	void	removeTaskItem(int nTaskID, int nItemIndex);
	// 刷新任务条件
	void	refreshTask(EmCompleteCondition eCondition, int nParam0, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0);
	// 给完成任务奖励
	void	giveCompleteTaskReward(int nTaskID);
	// 给接受任务奖励
	void	giveAcceptTaskReward(int nTaskID);
	// 给任务奖励
	void	giveTaskReward(CTaskConfig::TASK_PRIZE_LIST& rPrizeList);
	// 检查是否可以得到奖励
	int		checkTaskReward(int nTaskID);

public:
	/// 玩家身上是否有这个任务
	bool checkPlayerHasTask(int nTaskID);
	/// 通过任务ID得到任务
	CPlayerTask* getPlayerTask(int nTaskID);
	void setFromPB(PBTaskList* pbTaskList);
	void createToPB(PBTaskList* pbTaskList);
	/// 加入玩家任务
	void pushBackPlayerTask(int nTaskID);
	bool checkPlayerTaskListFull()
	{
		if (mTaskList.size() >= MAX_PLAYER_TASK_NUM)
		{
			return true;
		}
		return false;
	}
	void resetCompleteTask(int nTaskID)
	{
		mCompleteTasks.clearBit(nTaskID);
	}

	void completeTask(int nTaskID);
	bool checkTaskComplete(int nTaskID);
	void removeTask(int nTaskID);

private:
	/// 所有已经完成的任务
	Myth::CBitSet<MAX_TASK_ID>		mCompleteTasks;
	/// 已经接受的任务列表
	PLAYER_TASK_LIST				mTaskList;
};
#endif