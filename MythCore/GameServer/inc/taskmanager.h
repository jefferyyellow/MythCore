#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__
#include "commontype.h"
#include "simplelock.h"
#include <list>
enum EmTaskType
{
	emTaskType_None			= 0,			// δ֪
	emTaskType_DB			= 1,			// DB
	emTaskType_LocalLog		= 2,			// ������־
	emTaskType_Scene		= 4,			// ����
};

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