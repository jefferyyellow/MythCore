#include "mysqldatabase.h"
#include "mysqlqueryresult.h"
#include <errmsg.h>
namespace Myth
{
	uint16 CMysqlDataBase::mDBCount = 0;
	CMysqlDataBase::CMysqlDataBase()
	{
		if (0 == mDBCount++)
		{
			// ��ʼ�����ݿ�
			 mysql_library_init(-1, NULL, NULL);
		}
	}

	CMysqlDataBase::~CMysqlDataBase()
	{
		if (NULL != mMysql)
		{
			mysql_close(mMysql);
		}

		if (--mDBCount == 0)
		{
			// ж��
			mysql_library_end();
		}
	}

	int CMysqlDataBase::connectMysqlServer(char* pHost, char* pUserName, char* pPasswd, char* pDataBase, int nPort, char* pUnixSocket)
	{
		if (NULL == pHost || NULL == pUserName || NULL == pPasswd || NULL == pDataBase)
		{
			return -1;
		}

		MYSQL *pMysqlInit = mysql_init(NULL);
		if (NULL == pMysqlInit)
		{
			return -1;
		}

		if (0 == nPort)
		{
#ifdef MYTH_OS_WINDOWS
			unsigned int opt = MYSQL_PROTOCOL_PIPE;
			mysql_options(pMysqlInit,MYSQL_OPT_PROTOCOL,(char const*)&opt);
#else
			unsigned int opt = MYSQL_PROTOCOL_SOCKET;
			mysql_options(mysqlInit, MYSQL_OPT_PROTOCOL, (char const*)&opt);
#endif
		}

		// ����mysql
		mMysql = mysql_real_connect(pMysqlInit, pHost, pUserName,
			pPasswd, pDataBase, nPort, pUnixSocket, CLIENT_MULTI_RESULTS);

		if (NULL == mMysql)
		{
			return -1;
		}

		// ����mysql��������Ϣ�Ա�����
		strncpy(mHost, pHost, sizeof(mHost) - 1);
		strncpy(mUserName, pUserName, sizeof(mUserName) - 1);
		strncpy(mPasswd, pPasswd, sizeof(mPasswd) - 1);
		strncpy(mDataBase, pDataBase, sizeof(mDataBase) - 1);
		if (NULL != pUnixSocket)
		{
			strncpy(mUnixSocket, pUnixSocket, sizeof(mUnixSocket) - 1);
		}
		mPort = nPort;
		return 0;
	}
	
	int CMysqlDataBase::query(const char *pSql, CMysqlQueryResult& rQueryResult)
	{
		int nResult = query(pSql);
		if (0 != nResult)
		{
			return nResult;
		}

		MYSQL_RES *pResult = mysql_store_result(mMysql);
		uint32 nRowCount = (uint32)mysql_affected_rows(mMysql);
		uint32 nFieldCount = mysql_field_count(mMysql);

		if (NULL == pResult)
		{
			return -1;
		}
		// ��ʼ����ѯ���
		rQueryResult.init(pResult, nRowCount, nFieldCount);
		rQueryResult.nextRow();
		return 0;
	}

	int CMysqlDataBase::query(const char *pSql)
	{
		if (NULL == mMysql || NULL == pSql)
		{
			return -1;
		}

		int ret = mysql_query(mMysql, pSql);
		if (ret)
		{
			int nErrorNo = mysql_errno(mMysql);
			// �����mysql�������Ѿ�ʧЧ������mysql�Ѿ��������Զ�����
			if (nErrorNo == CR_SERVER_GONE_ERROR || nErrorNo == CR_SERVER_LOST || nErrorNo == CR_UNKNOWN_ERROR)
			{
				if (mMysql)
				{
					mysql_close(mMysql);
				}

				// ����mysql
				if (0 == connectMysqlServer(mHost, mPasswd, mPasswd, mDataBase, mPort, mUnixSocket))
				{
					// ����ִ�в�ѯ
					ret = mysql_query(mMysql, pSql);
				}
			}

			if (0 != ret)
			{
				return -1;
			}
		}
		return 0;
	}
}

