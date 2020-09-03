#ifndef __JOB_H__
#define __JOB_H__
#include "thread.h"
#include "taskmanager.h"
enum EmJobID
{
	emJobID_None	= 0,
	emJobID_Scene	= 1,		// ����job
	emJobID_DB		= 2,		// ���ݿ�job
	emJobID_Log		= 3,		// ������־job
	emJobID_Plat	= 4,		// ƽ̨job
	emJobID_Rank	= 5,		// ���а�job
};

template<int BaseCount, int Increment>
class CJob : public IJob
{
public:
	CJob()
	{
		init();
	}
	~CJob()
	{
	}

	void	init()
	{
        mLogTime = 0;
        mExited = false;
	}
public:
	virtual void doing(int uParam) = 0;
	void pushTask(CInternalMsg* pMsg)
	{
		mTaskManager.pushTask(pMsg);
	}

	CInternalMsg*	popTask()
	{
		return mTaskManager.popTask();
	}
	
	bool getExited() const { return mExited; }
	void setExited(bool nValue) { mExited = nValue; }

protected:
	time_t								mLogTime;
	CTaskManager<BaseCount, Increment>	mTaskManager;
	/// �˳����
	bool								mExited;
};
#endif