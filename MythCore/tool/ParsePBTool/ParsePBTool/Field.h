#ifndef __FIELD_H__
#define __FIELD_H__
#include <string>
#include <stdlib.h>
#include <stdio.h>
enum TmDataType
{
	emDataType_Unknow		= 0x00,
	emDataType_String		= 0x01,
	emDataType_Integer		= 0x02,
	emDataType_Float		= 0x03,
	emDataType_Bool		= 0x04,
	emDataType_Blob		=	0x05,
};

class Field
{
public:
	Field();
	Field(Field &f);
	Field(const char* pValue, TmDataType emDataType);
	~Field();

public:
	const char* GetString()const {return mpValue;}
	std::string GetStdString()const {return mpValue != NULL ? mpValue : "";}

	float		GetFloat() const {return NULL != mpValue ? atof(mpValue) : 0.0f;}

	int			GetInt() const {return NULL != mpValue ? atoi(mpValue) : 0;}

	short		GetShort() const {return NULL != mpValue ? atoi(mpValue) : 0;}
	
	char		GetByte() const {return NULL != mpValue ? atoi(mpValue) : 0;}

	long long	GetLongLong() const 
	{
		if (NULL == mpValue)
		{
			return 0;
		}
		long long int temp;
		sscanf(mpValue, "%lld", &temp);
		return temp;
	}
public:
	TmDataType GetDataType(){return mDataType;}
	void SetDataType(TmDataType emDataType){mDataType = emDataType;}

	char* GetValue(){return mpValue;}
	void SetValue(const char* pValue);
	void SetValue(const char* pValue, int len);

	short GetValueLen(){return mValueLen;}
	void SetValueLen(short nValueLen){mValueLen = nValueLen;}
private:
	char* mpValue;
	short mValueLen;
	TmDataType mDataType;
};
#endif
