#ifndef __TASKUNIT_H__
#define __TASKUNIT_H__
#include "playersubunit.h"
#include "bit_set.h"
#include "messagefactory.h"
#include "taskconfig.h"
class PBTaskList;
/// �������
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
	/// ����ID
	short			mTaskID;
	/// �������
	int				mTaskParam[MAX_COMPLETE_CONDITION];
	/// ��������
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

	/// ���������������
	void onAbortTaskRequest(Message* pMessage);
	/// ���ͷ��������Ӧ
	void sendAbortTaskResponse(int nResult, int nTaskID);
	/// ���͸����������֪ͨ
	void sendUpdateTaskProcessNotify(int nTaskID, int nCondIndex, int nParam);

	// ����Ƿ���Խ�������
	int		checkAcceptTask(int nTaskID);
	// ����Ƿ����������������
	int		checkAcceptCondition(EmAcceptCondition eCondition, int nParam1, int nParam2);
	// ����Ƿ���Թر�����
	int		checkCloseTask(int nTaskID);
	// ����Ƿ�����ر���������
	int		checkCloseCondition(CTaskCondition& rCondition);
	// ����Ƿ�����ύ����
	int		checkCommitTask(int nTaskID, int nItemIndex);
	// ����Ƿ������ύ��������
	int		checkCommitCondition(CPlayerTask* pPlayerTask, int nParamIndex, CTaskCondition& rCondition, int nItemIndex);
	// �۳��������
	void	removeTaskItem(int nTaskID, int nItemIndex);
	// ˢ����������
	void	refreshTask(EmCompleteCondition eCondition, int nParam0, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0);
	// �����������
	void	giveCompleteTaskReward(int nTaskID);
	// ������������
	void	giveAcceptTaskReward(int nTaskID);
	// ��������
	void	giveTaskReward(CTaskConfig::TASK_PRIZE_LIST& rPrizeList);
	// ����Ƿ���Եõ�����
	int		checkTaskReward(int nTaskID);

public:
	/// ��������Ƿ����������
	bool checkPlayerHasTask(int nTaskID);
	/// ͨ������ID�õ�����
	CPlayerTask* getPlayerTask(int nTaskID);
	void setFromPB(PBTaskList* pbTaskList);
	void createToPB(PBTaskList* pbTaskList);
	/// �����������
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
	/// �����Ѿ���ɵ�����
	Myth::CBitSet<MAX_TASK_ID>		mCompleteTasks;
	/// �Ѿ����ܵ������б�
	PLAYER_TASK_LIST				mTaskList;
};
#endif