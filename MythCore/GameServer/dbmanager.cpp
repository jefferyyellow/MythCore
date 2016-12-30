#include "dbmanager.h"
#include "internalmsg.h"
CDBManager::CDBManager()
{

}

CDBManager::~CDBManager()
{

}

void CDBManager::pushTask(CInternalMsg* pMsg)
{
	mLock.lock();
	mTaskList.push_back(pMsg);
	mLock.unlock();
}

CInternalMsg* CDBManager::popTask()
{
	mLock.lock();
	if (mTaskList.empty())
	{
		return NULL;
	}
	CInternalMsg* pMsg = mTaskList.front();
	mTaskList.pop_front();
	mLock.unlock();
	return pMsg;
}