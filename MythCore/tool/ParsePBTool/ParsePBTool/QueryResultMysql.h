#ifndef __QUERYRESULTMYSQL_H__
#define __QUERYRESULTMYSQL_H__

#include "Field.h"
#include <string>
#include <map>
#include <mysql.h>

typedef std::map<int, std::string> FieldNames;
class CQueryResultMysql
{
public:
	CQueryResultMysql(MYSQL_RES* result, int nRowCount, int nFieldCount);
	~CQueryResultMysql();

	// 通过名字得到字段索引
	int GetFieldIndex(const std::string &name)const
	{
		for (FieldNames::const_iterator iter = mFieldNames.begin(); iter != mFieldNames.end(); ++ iter)
		{
			if (iter->second == name)
			{
				return iter->first;
			}
		}

		return -1;
	}

	Field* Fetch()const {return mCurrentRow;}
	const Field& operator [](int nIndex)const 
	{
		return mCurrentRow[nIndex];
	}
	const Field& operator [](const std::string& name) const
	{
		return mCurrentRow[GetFieldIndex(name)];
	}
	
	bool NextRow();

public:
	int GetFiledCount()const {return mFieldCount;}
	void SetFieldCount(int nFieldCount){mFieldCount = nFieldCount;}

	int GetRowCount(){return mRowCount;}
	void SetRowCount(int nRowCount){mRowCount = nRowCount;}

	Field* GetCurrentRow(){return mCurrentRow;}

	FieldNames const& GetFiledNames()const {return mFieldNames;}


private:
	TmDataType ConvertNativeType(enum_field_types mysqlType) const;
	void EndQuery();

private:
	Field*		mCurrentRow;
	int			mFieldCount;
	int			mRowCount;
	FieldNames	mFieldNames;
	MYSQL_RES*	mResult;
};

#endif
