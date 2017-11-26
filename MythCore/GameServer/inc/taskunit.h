#ifndef __TASKUNIT_H__
#define __TASKUNIT_H__
#include "playersubunit.h"
#include "bit_set.h"
#define		MAX_TASK_ID		4096		// 最大的任务ID
#define		MAX_TASK_NUM	20			// 最多的可接任务数目
#define		MAX_TASK_PARAM	4			// 最多的任务参数数目
/// 任务条件类型
enum EmTaskCondType
{
	emTaskCondType_None			= 0,	// 无类型
	emTaskCondType_KillOgre		= 1,	// 刷怪	参数1：怪物ID，参数2：怪物数目
	emTaskCondType_ObtainItem	= 2,	// 获得道具 参数1：道具ID，参数2：道具数目
};

/// 玩家任务
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
	// 任务ID
	short			getTaskID() const { return mTaskID; }
	void			setTaskID(short nValue) { mTaskID = nValue; }

	// 任务进度
	short			getTaskProgress() const { return mTaskProgress; }
	void			setTaskProgress(short nValue) { mTaskProgress = nValue; }

	// 任务类型
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
	/// 任务ID
	short			mTaskID;
	/// 任务进度
	short			mTaskProgress;
	/// 任务条件类型
	EmTaskCondType	mTaskCondType;
	/// 任务参数
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
	/// 所有已经完成的任务
	Myth::CBitSet<MAX_TASK_ID>		mCompleteTasks;
	/// 最大的已经完成的任务
	short							mMaxCompleteTaskID;
	/// 已经接受的任务列表
	CPlayerTask						mTaskList[MAX_TASK_NUM];
	/// 已经接受任务列表数目
	int								mTaskListNum;
};
#endif