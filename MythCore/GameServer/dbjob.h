#ifndef __DBJOB_H__
#define __DBJOB_H__
#include "job.h"
#include "mysqldatabase.h"
using namespace Myth;

class CInternalMsg;
class CDBJob : public CJob<200, 20>
{
public:
	CDBJob();
	~CDBJob();
	int		init(char* pHost, char* pUserName, char* pPasswd, char* pDataBase, int nPort, char* pUnixSocket);

public:
	virtual void doing(uint32 uParam);

private:
	void	onTask(CInternalMsg* pMsg);
	/// Íæ¼ÒµÇÂ½ÓÎÏ·
	void	OnIMPlayerLoginRequest(CInternalMsg* pMsg);
	/// 

private:
	CMysqlDataBase	mDataBase;
};
#endif
