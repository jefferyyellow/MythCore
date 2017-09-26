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

	// ����������������
	strncpy(mHost, pHost, sizeof(mHost) - 1);
	mPort = nPort;
	strncpy(mUser, pUser, sizeof(mUser) - 1);
	strncpy(mPasswd, pPasswd, sizeof(mPasswd) - 1);
	strncpy(mDataBase, pDataBase, sizeof(mDataBase) - 1);

	// �������Ӻ�mMysql �� tpMysqlInit ָ��ͬһ���ڴ�
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


// �л���ĳһ�����µ����ݿ�
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
				printf("�Բ�ǡ����˳��ִ�������� \n");
				break;
			}
		case CR_SERVER_GONE_ERROR:
			{
				printf("MySQL������������ \n");
				break;
			}
		case CR_SERVER_LOST:
			{
				printf("�ڲ�ѯ�����У�������������Ӷ�ʧ \n");
				break;
			}
		case CR_UNKNOWN_ERROR:
			{
				printf("����δ֪���� \n");
				break;
			}
		default:
			{
				printf("Error %d \n", tResult );
				break;
			}
		}*/
		printf("�л����ݿ�ʧ�� .\n");
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

/*									--����������д��--
������������Ϊ�����ľ���ͼƬ��һЩ�ļ���Ϣ����Ȼ�����ﲻ����������Ϣ����ȷʵ�Ƕ��������ݡ�
���岽�裺
1��  ����һ��buffer�������飩���洢sql���
2��  ���漰������������֮ǰ��sql�����ӵ�buffer�У�����sprintf��strcpy�ȡ�
3��  ��mysql_real_escape_string()������Ӷ��������ݵ�buffer�С�
4��  ����ʣ���sql��䣬�γ�������sql��䡣
5��  ����mysql_real_query()������ִ��sql��䡣*/
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
