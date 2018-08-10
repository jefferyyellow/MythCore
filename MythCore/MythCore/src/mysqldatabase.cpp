#include "mysqldatabase.h"
#include "mysqlqueryresult.h"
#include <errmsg.h>
namespace Myth
{
	short CMysqlDataBase::mDBCount = 0;
	CMysqlDataBase::CMysqlDataBase()
	{
		init();
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
		//if (NULL != mMysqlInit)
		//{
		//	mysql_close(mMysqlInit);
		//}

		if (--mDBCount == 0)
		{
			// ж��
			mysql_library_end();
		}
		mMysql = NULL;
		mMysqlInit = NULL;
	}
	
	void CMysqlDataBase::init()
	{
		mMysql = NULL;
		mMysqlInit = NULL;
		mHost[0] = '\0';
		mUserName[0] = '\0';
		mPasswd[0] = '\0';
		mDataBase[0] = '\0';
		mUnixSocket[0] = '\0';
		mPort = 0;
	}

	int CMysqlDataBase::connectMysqlServer(const char* pHost, const char* pUserName, const char* pPasswd, const char* pDataBase, int nPort, const char* pUnixSocket)
	{
		if (NULL == pHost || NULL == pUserName || NULL == pPasswd || NULL == pDataBase)
		{
			return -1;
		}

		mMysqlInit = mysql_init(NULL);
		if (NULL == mMysqlInit)
		{
			return -1;
		}

		if (0 == nPort)
		{
#ifdef MYTH_OS_WINDOWS
			unsigned int opt = (unsigned int)MYSQL_PROTOCOL_PIPE;
			mysql_options(mMysqlInit,MYSQL_OPT_PROTOCOL,(char const*)&opt);
#else
			unsigned int opt = MYSQL_PROTOCOL_SOCKET;
			mysql_options(mMysqlInit, MYSQL_OPT_PROTOCOL, (char const*)&opt);
#endif
		}

		// ����mysql
		mMysql = mysql_real_connect(mMysqlInit, pHost, pUserName,
			pPasswd, pDataBase, (unsigned int)nPort, pUnixSocket, CLIENT_MULTI_RESULTS);

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
		int nRowCount = (int)mysql_affected_rows(mMysql);
		int nFieldCount = (int)mysql_field_count(mMysql);

		if (NULL == pResult)
		{
			return 0;
		}
		// ��ʼ����ѯ���
		rQueryResult.init(pResult, nRowCount, nFieldCount);
		rQueryResult.nextRow();
		return 0;
	}

	/// ��ѯ������������ֽ�����
	int CMysqlDataBase::query(const char *pSql, byte* pBuffer, int& rLength, int& rRowNum, int& rColNum)
	{
		int nResult = query(pSql);
		if (0 != nResult)
		{
			return nResult;
		}


		int nMaxSize = rLength;
		rLength = 0;
		MYSQL_RES *pResult = mysql_store_result(mMysql);
		int nRowCount = (int)mysql_affected_rows(mMysql);
		int nFieldCount = (int)mysql_field_count(mMysql);

		rRowNum = nRowCount;
		rColNum = nFieldCount;
		if (NULL == pResult)
		{
			return 0;
		}

		unsigned long* pValueLength = (unsigned long*)pBuffer;
		rLength = (int)(nRowCount * nFieldCount * sizeof(unsigned long));
		byte* pValue = pBuffer + rLength;

		for (int i = 0; i < nRowCount; ++ i)
		{
			MYSQL_ROW tRow = mysql_fetch_row(pResult);
			unsigned long *pColumn = mysql_fetch_lengths(pResult);
			if (NULL == tRow || NULL == pColumn)
			{
				break;
			}
			
			for (int nCount = 0; nCount < nFieldCount; ++ nCount)
			{
				rLength += (int)(pColumn[nCount] + 1);
				if (rLength > nMaxSize)
				{
					mysql_free_result(pResult);
					clearResult();
					rLength = 0;
					return -1;
				}
				*pValueLength = pColumn[nCount];
				pValueLength += 1;
				if (NULL != tRow[nCount])
				{
					memcpy(pValue, tRow[nCount], pColumn[nCount] + 1);
					pValue += pColumn[nCount] + 1;
				}
				else
				{
					*pValue = '\0';
					pValue += 1;
				}
			}
		}
		mysql_free_result(pResult);
		clearResult();
		

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
			unsigned int nErrorNo = mysql_errno(mMysql);
			// �����mysql�������Ѿ�ʧЧ������mysql�Ѿ��������Զ�����
			if (nErrorNo == CR_SERVER_GONE_ERROR || nErrorNo == CR_SERVER_LOST || nErrorNo == CR_UNKNOWN_ERROR)
			{
				mysql_close(mMysql);

				// ����mysql
				if (0 == connectMysqlServer(mHost, mPasswd, mPasswd, mDataBase, mPort, mUnixSocket))
				{
					// ����ִ�в�ѯ
					ret = mysql_query(mMysql, pSql);
				}
			}

			return ret;
		}
		return 0;
	}
	void CMysqlDataBase::clearResult()
	{
		if (mMysql == NULL) return;
		// ����mysql 2014����ȥ������
		MYSQL_RES* tRes = NULL;
		while (mysql_next_result(mMysql) == 0)
		{
			if(NULL != (tRes = mysql_store_result(mMysql)))
			{
				mysql_free_result(tRes);
			}
			
		}
	}
}



