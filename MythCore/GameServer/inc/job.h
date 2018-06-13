#ifndef __JOB_H__
#define __JOB_H__
#include "thread.h"
#include "taskmanager.h"
enum EmJobID
{
	emJobID_None	= 0,
	emJobID_Scene	= 1,
	emJobID_DB		= 2,
	emJobID_Log		= 3,
};

template<int BaseCount, int Increment>
class CJob : public IJob
{
public:
	CJob()
	{
	}
	~CJob()
	{
	}

	void	init()
	{
        mLogTime = 0;
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
protected:
	time_t								mLogTime;
	CTaskManager<BaseCount, Increment>	mTaskManager;
};
#endif