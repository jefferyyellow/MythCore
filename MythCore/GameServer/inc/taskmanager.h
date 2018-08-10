#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__
#include "servercommon.h"
#include "simplelock.h"
#include <list>
enum EmJobTaskType
{
	emJobTaskType_None			= 0,			// δ֪
	emJobTaskType_DB			= 1,			// DB
	emJobTaskType_LocalLog		= 2,			// ������־
	emJobTaskType_Scene			= 3,			// ����
	emJobTaskType_Plat			= 4,			// ƽ̨
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