#ifndef __DBMODULE_H__
#define __DBMODULE_H__
#include "dbmessage.h"
#include "loginmodule.h"
class CDBModule : public CLogicModule, public CSingleton < CDBModule >
{
	friend class CSingleton < CDBModule >;
private:
	CDBModule(){};
	~CDBModule(){};

public:
	virtual		void	OnTimer(unsigned int nTickOffset);

public:
	/// —π»ÎDB»ŒŒÒ
	void			pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, char* pSql, ...);
	CDBResponse*	getDBResponse(){return &mDBResponse;}
	void			onDBSession();

private:

private:
	CDBRequest		mDBRequest;
	CDBResponse		mDBResponse;
};
#endif