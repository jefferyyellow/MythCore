#ifndef __LOGINJOB_H__
#define __LOGINJOB_H__
#include "job.h"
#include "hashmap.h"
using namespace Myth;
#define  MAKE_LOGIN_KEY(Account, ChannelID, WorldID) ((Account << 32) & (ChannelID << 16) & (WorldID))
class CLoginJob : public CJob<20, 20>
{
public:
	CLoginJob(){}
	~CLoginJob(){}
public:
	virtual void doing(uint32 uParam);

private:
	void	onTask(CInternalMsg* pMsg);

private:
	typedef CHashMap<uint64, uint32, 100, 100, 100> LOGIN_LIST;
	LOGIN_LIST mLoginList;
};
#endif