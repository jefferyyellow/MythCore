#ifndef __TASKUNIT_H__
#define __TASKUNIT_H__
#include "playersubunit.h"
#include "bit_set.h"
#define		MAX_TASK_ID		4096		// ��������ID
#define		MAX_TASK_NUM	20			// ���Ŀɽ�������Ŀ
#define		MAX_TASK_PARAM	4			// �������������Ŀ
/// ������������
enum EmTaskCondType
{
	emTaskCondType_None			= 0,	// ������
	emTaskCondType_KillOgre		= 1,	// ˢ��	����1������ID������2��������Ŀ
	emTaskCondType_ObtainItem	= 2,	// ��õ��� ����1������ID������2��������Ŀ
};

/// �������
class CPlayerTask
{
public:
	CPlayerTask()
	{
		mTaskID = 0;
		mTaskProgress = 0;
		mTaskCondType = emTaskCondType_None;
		memset(mTaskParam, 0, sizeof(mTaskParam));
	}
	~CPlayerTask()
	{

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
		if (nIndex < 0 || nIndex >= MAX_TASK_PARAM)
		{
			return 0;
		}
		return mTaskParam[nIndex];
	}

	void			setTaskParam(int nIndex, int nTaskParam)
	{
		if (nIndex < 0 || nIndex >= MAX_TASK_PARAM)
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
	int				mTaskParam[MAX_TASK_PARAM];
};

class CEntityPlayer;
class CTaskUnit : public CPlayerSubUnit < CEntityPlayer >
{
public:
	CTaskUnit(CEntityPlayer* pPlayer)
		: CPlayerSubUnit(pPlayer)
	{
		mMaxCompleteTaskID = 0;
		mTaskListNum = 0;
	}
	~CTaskUnit()
	{

	}

public:
	void							refreshTaskProgress(EmTaskCondType eTaskCondType, int nParam1, int nParam2);
	void							setTaskComplete(int nTaskID)
	{
		mCompleteTasks.setBit(nTaskID);
		if (nTaskID > mMaxCompleteTaskID)
		{
			mMaxCompleteTaskID = nTaskID;
		}
	}

public:
	short							getMaxCompleteTaskID() const { return mMaxCompleteTaskID; }
	void							setMaxCompleteTaskID(short nValue) { mMaxCompleteTaskID = nValue; }

private:
	/// �����Ѿ���ɵ�����
	Myth::CBitSet<MAX_TASK_ID>		mCompleteTasks;
	/// �����Ѿ���ɵ�����
	short							mMaxCompleteTaskID;
	/// �Ѿ����ܵ������б�
	CPlayerTask						mTaskList[MAX_TASK_NUM];
	/// �Ѿ����������б���Ŀ
	int								mTaskListNum;
};
#endif