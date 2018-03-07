#ifndef __JOB_H__
#define __JOB_H__
#include "thread.h"
#include "taskmanager.h"
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
	CTaskManager<BaseCount, Increment> mTaskManager;
};
#endif