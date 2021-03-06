#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__
#include "servercommon.h"
#include "simplelock.h"
#include <list>

using namespace Myth;
class CInternalMsg;
/// 数据库管理程序,管理各个数据库Job
class CTaskManager
{
public:
	CTaskManager(){}
	~CTaskManager(){}

public:
	void			pushTask(CInternalMsg* pMsg)
	{
		mLock.lock();
		mTaskList.push_back(pMsg);
		mLock.unlock();
	}
	CInternalMsg*	popTask()
	{
		mLock.lock();
		if (mTaskList.empty())
		{
			mLock.unlock();
			return NULL;
		}
		CInternalMsg* pMsg = mTaskList.front();
		mTaskList.pop_front();
		mLock.unlock();
		return pMsg;
	}

	bool empty()
	{
		mLock.lock();
		bool bResult = mTaskList.empty();
		mLock.unlock();
		return bResult;
	}

private:
	std::list<CInternalMsg*>		mTaskList;
	CSimpleLock						mLock;
};
#endif