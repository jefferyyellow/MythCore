#ifndef __INTERNALMSGPOOL_H__
#define __INTERNALMSGPOOL_H__
#include "blockmemory.h"
#include "internalmsg.h"
#include "singleton.h"
#include "simplelock.h"
#include "blockmemory.h"
using namespace Myth;
class CInternalMsgPool : public CSingleton<CInternalMsgPool>
{
	friend class CSingleton < CInternalMsgPool > ;
private:
	CInternalMsgPool(){}
	~CInternalMsgPool(){}

public:
	CInternalMsg*	allocMsg(int nMessageID);
	void			freeMsg(CInternalMsg* pMsg);

private:
	CBlockMemory<CIMLocalLogRequest, 20, 5>			mLocalLogRequest;
	CBlockMemory<CIMPlayerLoginRequest, 20, 5>		mPlayerLoginRequest;
	CBlockMemory<CIMPlayerLoginResponse, 20, 5>		mPlayerLoginResponse;

	Myth::CSimpleLock								mLock;
};
#endif