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

		/// 连接mysql
		int		connectMysqlServer(char* pHost, char* pUserName, char* pPasswd, char* pDataBase, int nPort, char* pUnixSocket);
		/// 查询，返回结果
		int		query(const char *pSql, CMysqlQueryResult& rQueryResult);
		/// 查询，将结果返回字节流中
		int		query(const char *pSql, byte* pBuffer, int& rLength, int& rRowNum, int& rColNum);

		/// 查询，不在乎结果
		int		query(const char *pSql);
		void	clearResult();

		/// 影响行数
		int		getAffectRows()
		{
			return (int)mysql_affected_rows(mMysql);
		}

		/// 字段数目
		int		getFieldCount()
		{
			return mysql_field_count(mMysql);
		}

		/// 做后插入ID，用于自增长列
		int		getLastInsertID()
		{
			return (int)mysql_insert_id(mMysql);
		}

		/// 最近的错误码
		int getLastErrorno()
		{
			return mysql_errno(mMysql);
		}

		/// 最近错误描述
		const char* getLastError()
		{
			return mysql_error(mMysql);
		}

	private:
		/// 引用计数
		static short	mDBCount;
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
		int				mPort;
	};
}
#endif