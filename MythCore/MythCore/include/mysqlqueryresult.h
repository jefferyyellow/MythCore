#ifndef __MYSQLQUERYRESULT_H__
#define __MYSQLQUERYRESULT_H__

#include "mysqlfield.h"
#include <mysql.h>

namespace Myth
{
	enum EmMysqlDataType
	{
		emMysqlDataType_Unknow		= 0,
		emMysqlDataType_String		= 1,
		emMysqlDataType_Int			= 2,
		emMysqlDataType_Int64		= 3,
		emMysqlDataType_Float		= 4,
		emMysqlDataType_Double		= 5,
		emMysqlDataType_Blob		= 6,
	};

	#define MAX_ROW_FIELD_NUM	128
	class CMysqlDataBase;
	class CMysqlQueryResult
	{
	public:
		CMysqlQueryResult(CMysqlDataBase* pMysqlDataBase, bool bProcedure);
		~CMysqlQueryResult();

		int init(MYSQL_RES *pResult, int nRowCount, int nFieldCount);
		int nextRow();
		EmMysqlDataType convertNativeType(enum_field_types mysqlType) const;
		void clear();

	public:
		CMysqlField*	getField(unsigned int nIndex)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return NULL;
			}
			return &mField[nIndex];
		}

		int			getFileValueLen(unsigned int nIndex)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return 0;
			}

			return mField[nIndex].getValueLen();
		}


		char*			getFileValue(unsigned int nIndex)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return 0;
			}

			return mField[nIndex].getValue();
		}

		void			setField(unsigned int nIndex, char* pValue, int nValueLen)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return;
			}
			mField[nIndex].setValue(pValue);
			mField[nIndex].setValueLen(nValueLen);
		}

		/// �õ�����������
		EmMysqlDataType	getFieldDataType(unsigned int nIndex)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return emMysqlDataType_Unknow;
			}
			return mFieldDataType[nIndex];
		}

		/// ��������������
		void			setFieldDataType(unsigned int nIndex, EmMysqlDataType eType)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return ;
			}
			mFieldDataType[nIndex] = eType;
		}

		/// �õ�����Ŀ
		int				getFieldCount(){return mFieldCount;}
		void			setFieldCount(int nFieldCount){mFieldCount = nFieldCount;}

		/// �õ�����Ŀ
		int				getRowCount(){return mRowCount;}
		void			setRowCount(int nRowCount){mRowCount = nRowCount;}

	private:
		/// mysql
		CMysqlDataBase*	mMysqlDataBase;
		/// mysql�Ľ��
		MYSQL_RES*		mResult;
		/// ������	
		CMysqlField		mField[MAX_ROW_FIELD_NUM];
		/// ������
		EmMysqlDataType	mFieldDataType[MAX_ROW_FIELD_NUM];
		/// ����Ŀ		
		int				mFieldCount;
		/// ����Ŀ
		int				mRowCount;
		/// �Ƿ�ִ�д洢����
		bool			mProcedure;
	};
}
#endif