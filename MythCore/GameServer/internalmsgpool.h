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
public:
	CInternalMsgPool();
	~CInternalMsgPool();

public:
	CInternalMsg*	allocMsg(int nMessageID);
	void			freeMsg(CInternalMsg* pMsg);

private:
	CBlockMemory<CPlayerLoginRequest, 20, 5>	mPlayerLoginRequest;
	Myth::CSimpleLock							mLock;
};
#endif