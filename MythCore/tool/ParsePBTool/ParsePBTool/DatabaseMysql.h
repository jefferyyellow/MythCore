#ifndef __DATABASEMYSQL_H__
#define __DATABASEMYSQL_H__
#include <mysql.h>
#include <errmsg.h>

#define MAX_CHAR_LENGTH 256
class CQueryResultMysql;
class CDatabaseMysql
{
public:
	CDatabaseMysql();
	~CDatabaseMysql();


public:
	bool Initialize(const char* pHost, int nPort, const char* pUser, const char* pPasswd, const char* pDataBase);
	CQueryResultMysql* Query(const char* sql);
	bool Execute(const char* sql);
	bool DirectExecute(const char* sql);

	long GetLastInsertID();
	void ClearResult();
	int AffectedRows();
	// 切换到某一连接下的数据库
	bool SwitchDataBase( const char* pDataBase );

public:
	int LastErrorNo()const {return mysql_errno(mMysql);}

private:
	MYSQL* mMysql;
	char	mHost[MAX_CHAR_LENGTH];
    int	mPort;
	char	mUser[MAX_CHAR_LENGTH];
	char	mPasswd[MAX_CHAR_LENGTH];
	char	mDataBase[MAX_CHAR_LENGTH];
	char mUnixSocket[MAX_CHAR_LENGTH];
};

#endif
