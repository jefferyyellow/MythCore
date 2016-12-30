#ifndef __DBMANAGER_H__
#define __DBMANAGER_H__
#include "singleton.h"
#include "commontype.h"
#include "list.h"
#include "simplelock.h"
using namespace Myth;
class CInternalMsg;
/// ���ݿ�������,����������ݿ�Job
class CDBManager : public CSingleton<CDBManager>
{
	friend class CSingleton < CDBManager > ;
private:
	CDBManager();
	~CDBManager();


public:
	void			pushTask(CInternalMsg* pMsg);
	CInternalMsg*	popTask();

private:
	Myth::CList<CInternalMsg*, 1000, 100>	mTaskList;
	Myth::CSimpleLock						mLock;
};
#endif