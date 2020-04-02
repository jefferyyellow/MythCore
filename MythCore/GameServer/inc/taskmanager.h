#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__
#include "servercommon.h"
#include "simplelock.h"
#include <list>

using namespace Myth;
class CInternalMsg;
/// ���ݿ�������,����������ݿ�Job
template<int BaseCount, int Increment>
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
	//CList<CInternalMsg*, BaseCount, Increment>	mTaskList;
	std::list<CInternalMsg*>		mTaskList;
	CSimpleLock						mLock;
};
#endif