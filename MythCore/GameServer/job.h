#ifndef __JOB_H__
#define __JOB_H__
#include "thread.h"
#include "taskmanager.h"
template<uint BaseCount, uint Increment>
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
	virtual void doing(uint32 uParam) = 0;
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