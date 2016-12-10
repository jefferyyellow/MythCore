#ifndef __MYSQLQUERYRESULT_H__
#define __MYSQLQUERYRESULT_H__

#include "mysqlfield.h"
#include <mysql.h>

namespace Myth
{
	enum TmMysqlDataType
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
	class CMysqlQueryResult
	{
	public:
		CMysqlQueryResult();
		~CMysqlQueryResult();

		int init(MYSQL_RES *pResult, uint32 nRowCount, uint32 nFieldCount);
		int nextRow();
		TmMysqlDataType convertNativeType(enum_field_types mysqlType) const;

	public:
		CMysqlField*	getField(unsigned int nIndex)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return NULL;
			}
			return &mField[nIndex];
		}

		void			setField(unsigned int nIndex, char* pValue, uint32 nValueLen)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return;
			}
			mField[nIndex].setValue(pValue);
			mField[nIndex].setValueLen(nValueLen);
		}

		/// �õ�����������
		TmMysqlDataType	getFieldDataType(unsigned int nIndex)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return emMysqlDataType_Unknow;
			}
			return mFieldDataType[nIndex];
		}

		/// ��������������
		void			setFieldDataType(unsigned int nIndex, TmMysqlDataType eType)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return ;
			}
			mFieldDataType[nIndex] = eType;
		}

		/// �õ�����Ŀ
		uint32			getFieldCount(){return mFieldCount;}
		void			setFieldCount(uint32 nFieldCount){mFieldCount = nFieldCount;}

		/// �õ�����Ŀ
		uint32			getRowCount(){return mRowCount;}
		void			setRowCount(uint32 nRowCount){mRowCount = nRowCount;}

	private:
		/// mysql�Ľ��
		MYSQL_RES		*mResult;
		/// ������	
		CMysqlField		mField[MAX_ROW_FIELD_NUM];
		/// ������
		TmMysqlDataType	mFieldDataType[MAX_ROW_FIELD_NUM];
		/// ����Ŀ		
		uint32			mFieldCount;
		/// ����Ŀ
		uint32			mRowCount;
	};
}
#endif