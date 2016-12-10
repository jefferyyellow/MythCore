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

		/// 得到列数据类型
		TmMysqlDataType	getFieldDataType(unsigned int nIndex)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return emMysqlDataType_Unknow;
			}
			return mFieldDataType[nIndex];
		}

		/// 设置列数据类型
		void			setFieldDataType(unsigned int nIndex, TmMysqlDataType eType)
		{
			if (nIndex >= MAX_ROW_FIELD_NUM)
			{
				return ;
			}
			mFieldDataType[nIndex] = eType;
		}

		/// 得到列数目
		uint32			getFieldCount(){return mFieldCount;}
		void			setFieldCount(uint32 nFieldCount){mFieldCount = nFieldCount;}

		/// 得到行数目
		uint32			getRowCount(){return mRowCount;}
		void			setRowCount(uint32 nRowCount){mRowCount = nRowCount;}

	private:
		/// mysql的结果
		MYSQL_RES		*mResult;
		/// 列数据	
		CMysqlField		mField[MAX_ROW_FIELD_NUM];
		/// 列类型
		TmMysqlDataType	mFieldDataType[MAX_ROW_FIELD_NUM];
		/// 列数目		
		uint32			mFieldCount;
		/// 行数目
		uint32			mRowCount;
	};
}
#endif