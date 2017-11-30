#ifndef __TASKUNIT_H__
#define __TASKUNIT_H__
#include "playersubunit.h"
#include "bit_set.h"
#include "messagefactory.h"
#include "taskconfig.h"
#define		MAX_PLAYER_TASK_NUM		20			// ���Ŀɽ�������Ŀ
#define		MAX_PLAYER_TASK_PARAM	4			// �������������Ŀ
/// ������������
enum EmTaskCondType
{
	emTaskCondType_None			= 0,	// ������
	emTaskCondType_Talk			= 1,	// �Ի�����
	emTaskCondType_KillOgre		= 2,	// ˢ��	����1������ID������2��������Ŀ
	emTaskCondType_ObtainItem	= 3,	// ��õ��� ����1������ID������2��������Ŀ
	emTaskCondTypeMax
};

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
		mTaskProgress = 0;
		mTaskCondType = emTaskCondType_None;
		memset(mTaskParam, 0, sizeof(mTaskParam));
	}
public:
	// ����ID
	short			getTaskID() const { return mTaskID; }
	void			setTaskID(short nValue) { mTaskID = nValue; }

	// �������
	short			getTaskProgress() const { return mTaskProgress; }
	void			setTaskProgress(short nValue) { mTaskProgress = nValue; }

	// ��������
	EmTaskCondType	getTaskCondType() const { return mTaskCondType; }
	void			setTaskCondType(EmTaskCondType nValue) { mTaskCondType = nValue; }

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
	short			mTaskProgress;
	/// ������������
	EmTaskCondType	mTaskCondType;
	/// �������
	int				mTaskParam[MAX_PLAYER_TASK_PARAM];
};

class CEntityPlayer;
class CTaskUnit : public CPlayerSubUnit
{
public:
	CTaskUnit(CEntityPlayer& rPlayer)
		: CPlayerSubUnit(rPlayer)
	{
		mMaxCompleteTaskID = 0;
		mTaskListNum = 0;
	}
	~CTaskUnit()
	{

	}

public:
	void							fireEvent(EmTaskCondType eTaskCondType, int nParam);
	void							setTaskComplete(int nTaskID)
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

	/// �õ�һ���յ�����
	int	getFreeTask();
private:
	/// �����Ѿ���ɵ�����
	Myth::CBitSet<MAX_TASK_ID>		mCompleteTasks;
	/// �����Ѿ���ɵ�����
	short							mMaxCompleteTaskID;
	/// �Ѿ����ܵ������б�
	CPlayerTask						mTaskList[MAX_PLAYER_TASK_NUM];
	/// �Ѿ����������б���Ŀ
	int								mTaskListNum;
};
#endif