#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__
#include "commontype.h"
#include "list.h"
#include "simplelock.h"

enum EmTaskType
{
	emTaskType_None			= 0,			// 未知
	emTaskType_DB			= 1,			// DB
	emTaskType_LocalLog		= 2,			// 本地日志
	emTaskType_Scene		= 4,			// 场景
};

using namespace Myth;
class CInternalMsg;
/// 数据库管理程序,管理各个数据库Job
template<uint BaseCount, uint Increment>
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

private:
	CList<CInternalMsg*, BaseCount, Increment>	mTaskList;
	CSimpleLock						mLock;
};
#endif