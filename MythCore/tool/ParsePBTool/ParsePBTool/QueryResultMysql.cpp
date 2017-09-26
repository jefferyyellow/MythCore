#ifdef WIN32
#include <Windows.h>
#endif // WINDOWS

#include <stdio.h>
#include "QueryResultMysql.h"

CQueryResultMysql::CQueryResultMysql(MYSQL_RES* result, int nRowCount, int nFieldCount)
:mRowCount(nRowCount), mFieldCount(nFieldCount),mResult(result)
{
	mCurrentRow = new Field[mFieldCount];
	if (NULL == mCurrentRow)
	{
		return;
	}
	MYSQL_FIELD* fields = mysql_fetch_field(mResult);
	for (int i = 0; i < mFieldCount; ++ i)
	{
		mFieldNames[i] = fields[i].name;
		mCurrentRow[i].SetDataType(ConvertNativeType(fields[i].type));
	}
}

CQueryResultMysql::~CQueryResultMysql()
{

	EndQuery();
}

bool CQueryResultMysql::NextRow()
{
	MYSQL_ROW row;
	if (NULL == mResult)
	{
		return false;
	}
	
	row = mysql_fetch_row(mResult);
	unsigned long *field_len = mysql_fetch_lengths(mResult);
	if (NULL == row || NULL == field_len)
	{
		EndQuery();
		return false;
	}

	for(int i = 0; i < mFieldCount; ++ i)
	{
		mCurrentRow[i].SetValue(row[i], field_len[i]);
	}
	return true;
}

TmDataType CQueryResultMysql::ConvertNativeType(enum_field_types mysqlType) const
{
	switch(mysqlType)
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
		{
			return emDataType_String;
			break;
		}
	case FIELD_TYPE_SHORT:
	case FIELD_TYPE_LONG:
	case FIELD_TYPE_INT24:
	case FIELD_TYPE_LONGLONG:
	case FIELD_TYPE_ENUM:
		{
			return emDataType_Integer;
			break;
		}
	case FIELD_TYPE_DECIMAL:
	case FIELD_TYPE_FLOAT:
	case FIELD_TYPE_DOUBLE:
		{
			return emDataType_Float;
			break;
		}
	case FIELD_TYPE_BLOB:
		{
			return emDataType_Blob;
			break;
		}
	default:
		{
			return emDataType_Unknow;
			break;
		}
	}

	return emDataType_Unknow;
}

void CQueryResultMysql::EndQuery()
{
	if (mCurrentRow)
	{
		delete []mCurrentRow;
		mCurrentRow = NULL;
	}
	if (mResult)
	{
		mysql_free_result(mResult);
		mResult = NULL;
	}
}
