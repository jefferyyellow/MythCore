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

		/// ����mysql
		int		connectMysqlServer(char* pHost, char* pUserName, char* pPasswd, char* pDataBase, int nPort, char* pUnixSocket);
		/// ��ѯ�����ؽ��
		int		query(const char *pSql, CMysqlQueryResult& rQueryResult);
		/// ��ѯ������������ֽ�����
		int		query(const char *pSql, byte* pBuffer, int& rLength, int& rRowNum, int& rColNum);

		/// ��ѯ�����ں����
		int		query(const char *pSql);
		void	clearResult();

		/// Ӱ������
		int		getAffectRows()
		{
			return (int)mysql_affected_rows(mMysql);
		}

		/// �ֶ���Ŀ
		int		getFieldCount()
		{
			return mysql_field_count(mMysql);
		}

		/// �������ID��������������
		int		getLastInsertID()
		{
			return (int)mysql_insert_id(mMysql);
		}

		/// ����Ĵ�����
		int getLastErrorno()
		{
			return mysql_errno(mMysql);
		}

		/// �����������
		const char* getLastError()
		{
			return mysql_error(mMysql);
		}
		
		MYSQL * GetMysql() const { return mMysql; }
	private:
		/// ���ü���
		static short	mDBCount;
		MYSQL			*mMysql;
		/// �������ݿ���Ϣ���ڶ�������
		/// ע�⣬Host��ֻ����IP��������������
		char			mHost[STR_LENGTH_64];
		/// �û���
		char			mUserName[STR_LENGTH_16];
		/// ����
		char			mPasswd[STR_LENGTH_16];
		/// ���ӵ����ݿ���
		char			mDataBase[STR_LENGTH_16];
		/// unix socket
		char			mUnixSocket[STR_LENGTH_32];
		/// �˿�
		int				mPort;
	};
}
#endif