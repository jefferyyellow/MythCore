#ifdef WIN32
#include <Windows.h>
#endif // WINDOWS

#include "DatabaseMysql.h"
#include "QueryResultMysql.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

CDatabaseMysql::CDatabaseMysql() : mMysql(NULL), mPort(0)
{
	memset( mHost, 0, MAX_CHAR_LENGTH);
	memset( mUser, 0, MAX_CHAR_LENGTH);
	memset( mPasswd, 0, MAX_CHAR_LENGTH);
	memset( mDataBase, 0, MAX_CHAR_LENGTH);
	memset( mUnixSocket, 0, MAX_CHAR_LENGTH);
	mysql_library_init(NULL, 0, 0);
}

CDatabaseMysql::~CDatabaseMysql()
{
	if (NULL != mMysql)
	{
		mysql_close(mMysql);
	}

	mysql_library_end();
}

bool CDatabaseMysql::Initialize(const char* pHost, int nPort, const char* pUser, const char* pPasswd, const char* pDataBase)
{
	if (NULL == pHost || 0 == nPort || NULL == pUser || NULL == pPasswd || NULL == pDataBase)
	{
		return false;
	}

	char szUnixSocket[32] = {0};
	const char* pUnixSocket = NULL;

	MYSQL* tpMysqlInit = mysql_init(NULL);
	if (NULL == tpMysqlInit)
	{
		printf("mysql init failed!");
		return false;
	}

	mysql_options(tpMysqlInit, MYSQL_SET_CHARSET_NAME, "utf8");
	if (strcmp(pHost, ".") == 0)
	{
		unsigned int opt = MYSQL_PROTOCOL_SOCKET;
		mysql_options(tpMysqlInit, MYSQL_OPT_PROTOCOL,(char const*)&opt);
		pHost = "localhost";
		nPort = 0;
		//pUnixSocket = itoa(nPort, szUnixSocket, 10);
		sprintf(szUnixSocket, "%d", 10);
		pUnixSocket = szUnixSocket;
		memcpy( mUnixSocket, szUnixSocket, sizeof(szUnixSocket) );
	}
	else
	{
		pUnixSocket = NULL;
	}

	// 保存配置用于重连
	strncpy(mHost, pHost, sizeof(mHost) - 1);
	mPort = nPort;
	strncpy(mUser, pUser, sizeof(mUser) - 1);
	strncpy(mPasswd, pPasswd, sizeof(mPasswd) - 1);
	strncpy(mDataBase, pDataBase, sizeof(mDataBase) - 1);

	// 建立连接后，mMysql 和 tpMysqlInit 指向同一块内存
	mMysql = mysql_real_connect(tpMysqlInit, mHost, mUser, mPasswd, pDataBase, mPort, mUnixSocket, CLIENT_MULTI_RESULTS);
	if(mMysql != NULL )
	{
		if ( !mysql_autocommit(mMysql, 1) )
		{
			printf("Auto commit success fully set to 1\n");
		}
		else
		{
			printf("Auto commit not set to 1\n");
		}
		return true;
	}
	else
	{
		mysql_close(tpMysqlInit);
		printf("Count not connect to mysql database at %s: %s\n", mHost, mysql_error(tpMysqlInit));
		return false;
	}
}


// 切换到某一连接下的数据库
bool CDatabaseMysql::SwitchDataBase( const char* pDataBase )
{
	if ( pDataBase == NULL || mMysql == NULL )
	{
		return false;
	}
	int tResult = mysql_select_db( mMysql, pDataBase );
	if( tResult != 0 )
	{
		/*switch( tResult )
		{
		case CR_COMMANDS_OUT_OF_SYNC:
			{
				printf("以不恰当的顺序执行了命令 \n");
				break;
			}
		case CR_SERVER_GONE_ERROR:
			{
				printf("MySQL服务器不可用 \n");
				break;
			}
		case CR_SERVER_LOST:
			{
				printf("在查询过程中，与服务器的连接丢失 \n");
				break;
			}
		case CR_UNKNOWN_ERROR:
			{
				printf("出现未知错误 \n");
				break;
			}
		default:
			{
				printf("Error %d \n", tResult );
				break;
			}
		}*/
		printf("切换数据库失败 .\n");
		return false;
	}
	return true;
}

CQueryResultMysql* CDatabaseMysql::Query(const char* sql)
{
	if(NULL == mMysql)
	{
		return NULL;
	}

	MYSQL_RES *result = NULL;
	int nRowCount = 0;
	int nFieldCount = 0;

	int nRet = mysql_query(mMysql, sql);
	if (nRet != 0)
	{
		int nErrorNo = mysql_errno(mMysql);
		if (nErrorNo == CR_SERVER_GONE_ERROR || nErrorNo == CR_SERVER_LOST || nErrorNo == CR_UNKNOWN_ERROR)
		{
			if (NULL != mMysql)
			{
				mysql_close(mMysql);
			}
			if (Initialize(mHost, mPort, mUser, mPasswd, mDataBase))
			{
				nRet = mysql_query(mMysql, sql);
			}
		}
		if (nRet)
		{
			return NULL;
		}
	}

	result = mysql_store_result(mMysql);
	nRowCount = mysql_affected_rows(mMysql);
	nFieldCount = mysql_field_count(mMysql);

	if(NULL == result)
	{
		return NULL;
	}

	CQueryResultMysql* pQueryResult = new CQueryResultMysql(result, nRowCount, nFieldCount);
	if (NULL == pQueryResult)
	{
		return NULL;
	}
	pQueryResult->NextRow();
	return pQueryResult;
}

bool CDatabaseMysql::Execute(const char* sql)
{
	if (NULL == mMysql)
	{
		return false;
	}

	return true;
}

bool CDatabaseMysql::DirectExecute(const char* sql)
{
	if (NULL == mMysql)
	{
		return false;
	}

	int nRet = mysql_query(mMysql, sql);

	if (0 != nRet)
	{
		int nErrorNo = mysql_errno(mMysql);
		if (nErrorNo == CR_SERVER_GONE_ERROR || nErrorNo == CR_SERVER_LOST || nErrorNo == CR_UNKNOWN_ERROR)
		{
			if (NULL != mMysql)
			{
				mysql_close(mMysql);
			}

			if (Initialize(mHost, mPort, mUser, mPasswd, mDataBase))
			{
				nRet = mysql_query(mMysql, sql);
			}
		}
		if (nRet)
		{
			return false;
		}
	}

	return true;
}

/*									--二进制数据写入--
二进制数据最为常见的就是图片等一些文件信息。虽然我这里不是这类型信息，但确实是二进制数据。
具体步骤：
1、  定义一个buffer（如数组）来存储sql语句
2、  把涉及到二进制数据之前的sql语句添加到buffer中，可用sprintf或strcpy等。
3、  用mysql_real_escape_string()函数添加二进制数据到buffer中。
4、  加上剩余的sql语句，形成完整的sql语句。
5、  利用mysql_real_query()函数来执行sql语句。*/
/*bool CDatabaseMysql::DirectExecuteBlob( const char* sql, char* pBuf )
{
	if (NULL == mMysql || sql == NULL || pBuf == NULL )
	{
		return false;
	}

	char* tpEnd = NULL;

	mysql_real_escape_string();
	int nRet = mysql_real_query( mMysql, sql, );

	char query[1000]={"Insert into mytable values("};
	char *end;
	end=query;
	end+=30;
	end = strmov(query,"INSERT INTO mytable values(");
	*end++ = '\'';
	end += mysql_real_escape_string(MyData, end,"What's this",11);
	*end++ = '\'';
	*end++ = ',';
	*end++ = '\'';
	end += mysql_real_escape_string(MyData, end,"binary data: \4\r\n",16);
	*end++ = '\'';
	*end++ = ')';

	if (0 != nRet)
	{
		int nErrorNo = mysql_errno(mMysql);
		if (nErrorNo == CR_SERVER_GONE_ERROR || nErrorNo == CR_SERVER_LOST || nErrorNo == CR_UNKNOWN_ERROR)
		{
			if (NULL != mMysql)
			{
				mysql_close(mMysql);
			}

			if (Initialize(mHost, mPort, mUser, mPasswd, mDataBase))
			{
				nRet = mysql_query(mMysql, sql);
			}
		}
		if (nRet)
		{
			return false;
		}
	}

	return true;
}*/

long CDatabaseMysql::GetLastInsertID()
{
	return mysql_insert_id(mMysql);
}

void CDatabaseMysql::ClearResult()
{
	if (NULL == mMysql)
	{
		return;
	}

	MYSQL_RES* res = NULL;
	while(mysql_next_result(mMysql) == 0)
	{
		mysql_free_result(res);
	}
}

int CDatabaseMysql::AffectedRows()
{
	if (NULL == mMysql)
	{
		return 0;
	}
	return mysql_affected_rows(mMysql);
}
