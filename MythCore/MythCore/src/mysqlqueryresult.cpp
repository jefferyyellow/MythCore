#include "mysqlqueryresult.h"

namespace Myth
{
	CMysqlQueryResult::CMysqlQueryResult()
	{
		mResult = NULL;
		mFieldCount = 0;
		mRowCount = 0;
	}

	CMysqlQueryResult::~CMysqlQueryResult()
	{
		if (NULL != mResult)
		{
			mysql_free_result(mResult);
			mResult = NULL;
		}
		mFieldCount = 0;
		mRowCount = 0;
	}

	int CMysqlQueryResult::init(MYSQL_RES *pResult, uint32 nRowCount, uint32 nFieldCount)
	{
		if (NULL == pResult)
		{
			return -1;
		}
		if (nFieldCount > MAX_ROW_FIELD_NUM)
		{
			return -1;
		}
		// 如果以前就是遗留，先free
		if (NULL != mResult)
		{
			mysql_free_result(mResult);
			mResult = NULL;
		}

		mResult = pResult;
		mFieldCount = nFieldCount;
		mRowCount = nRowCount;

		 MYSQL_FIELD *pFields = mysql_fetch_fields(mResult);
		 for (uint32 i = 0; i < mFieldCount; i++)
		 {
			setFieldDataType(i, convertNativeType(pFields[i].type));
		 }
		 return 0;
	}

	int CMysqlQueryResult::nextRow()
	{
		if (NULL == mResult)
		{
			return -1;
		}

		MYSQL_ROW tRow = mysql_fetch_row(mResult);
		unsigned long *pColumn = mysql_fetch_lengths(mResult);
		if (NULL == tRow || NULL == pColumn)
		{
			return -1;
		}

		for (uint32 i = 0; i < mFieldCount; i++)
		{
			setField(i, tRow[i], pColumn[i]);
		}
		return 0;
	}

	TmMysqlDataType CMysqlQueryResult::convertNativeType(enum_field_types mysqlType) const
	{
		switch (mysqlType)
		{
			case FIELD_TYPE_TIMESTAMP:
			case FIELD_TYPE_DATE:
			case FIELD_TYPE_TIME:
			case FIELD_TYPE_DATETIME:
			case FIELD_TYPE_YEAR:
			case FIELD_TYPE_STRING:
			case FIELD_TYPE_VAR_STRING:
			case FIELD_TYPE_SET:
			case FIELD_TYPE_NULL:
				return emMysqlDataType_String;
			case FIELD_TYPE_BLOB:
				return emMysqlDataType_Blob;
			case FIELD_TYPE_TINY:
			case FIELD_TYPE_SHORT:
			case FIELD_TYPE_LONG:
			case FIELD_TYPE_INT24:
			case FIELD_TYPE_ENUM:
				return emMysqlDataType_Int;
			case FIELD_TYPE_LONGLONG:
				return emMysqlDataType_Int64;
			case FIELD_TYPE_DECIMAL:
			case FIELD_TYPE_FLOAT:
				return emMysqlDataType_Float;
			case FIELD_TYPE_DOUBLE:
				return emMysqlDataType_Double;
			default:
				return emMysqlDataType_Unknow;
		}
	}
}