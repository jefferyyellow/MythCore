#ifndef __MYSQLDATABASE_H__
#define __MYSQLDATABASE_H__
#include "commontype.h"
#include <mysql.h>

namespace Myth
{
	class CMysqlQueryResult;
	class CMysqlDataBase
	{
	public:
		CMysqlDataBase();
		~CMysqlDataBase();

		int		connectMysqlServer(char* pHost, char* pUserName, char* pPasswd, char* pDataBase, int nPort, char* pUnixSocket);
		int		query(const char *sql, CMysqlQueryResult& rQueryResult);
		int		query(const char *sql);
		int		getAffectRows()
		{
			if (NULL == mMysql)
			{
				return 0;
			}
			return mysql_affected_rows(mMysql);
		}

		int		getFieldCount()
		{
			if (NULL == mMysql)
			{
				return 0;
			}
			return mysql_field_count(mMysql);
		}

		int		getLastInsertID()
		{
			if (NULL == mMysql)
			{
				return 0;
			}
			return mysql_insert_id( mMysql );
		}

	private:
		/// 引用计数
		static uint16	mDBCount;
		MYSQL			*mMysql;
		/// 保持数据库信息用于断线重连
		/// 注意，Host不只是有IP，还可能是域名
		char			mHost[STRING_LENGTH_64];
		/// 用户名
		char			mUserName[STRING_LENGTH_16];
		/// 密码
		char			mPasswd[STRING_LENGTH_16];
		/// 连接的数据库名
		char			mDataBase[STRING_LENGTH_16];
		/// unix socket
		char			mUnixSocket[STRING_LENGTH_32];
		/// 端口
		uint32			mPort;
	};
}
#endif