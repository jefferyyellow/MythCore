#ifndef __TASKUNIT_H__
#define __TASKUNIT_H__
#include "playersubunit.h"
#include "bit_set.h"
#include "messagefactory.h"
#include "taskconfig.h"
#define		MAX_PLAYER_TASK_NUM		20			// ���Ŀɽ�������Ŀ
#define		MAX_PLAYER_TASK_PARAM	4			// �������������Ŀ

enum EmTaskState
{
	emTaskState_None			= 0,	// ������
	emTaskState_Accept			= 1,	// �����ѽ���
	emTaskState_Complete		= 2,	// ���������
	emTaskState_Failure			= 3,	// ������ʧ��
	emTaskStateMax
};

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
		mTaskStatus = 0;
		memset(mTaskParam, 0, sizeof(mTaskParam));
	}
public:
	// ����ID
	short			getTaskID() const { return mTaskID; }
	void			setTaskID(short nValue) { mTaskID = nValue; }

	// �������
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
	/// ����ID
	short			mTaskID;
	/// �������
	short			mTaskStatus;
	/// �������
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
	/// ��������Ƿ��ܽ���
	int	checkAcceptTask(int nTaskID);
	/// ������ܺ���
	void afterAcceptTask(int nTaskID);
	/// ��������Ƿ����ύ
	int checkSubmitTask(int nTaskID, short& bRepeated);

	/// ����ID�õ�������ϵ�����
	CPlayerTask* getPlayerTask(int nTaskID);
	/// ɾ��������ϵ�����
	void removeTask(int nTaskID);
private:
	/// �����Ѿ���ɵ�����
	Myth::CBitSet<MAX_TASK_ID>		mCompleteTasks;
	/// �����Ѿ���ɵ�����
	short							mMaxCompleteTaskID;
	/// �Ѿ����ܵ������б�
	PLAYER_TASK_LIST				mTaskList;
};
#endif