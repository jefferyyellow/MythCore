#include "Field.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Field::Field()
:mpValue(NULL),mDataType(emDataType_Unknow)
{

}

Field::Field(Field &f)
{
	const char* value = f.GetString();
	if (NULL == value)
	{
		mpValue = NULL;
		mDataType = f.GetDataType();
		return;
	}

	mpValue = new char[strlen(value) + 1];
	if (NULL == mpValue)
	{
		return;
	}
	strcpy(mpValue, value);
	mDataType = f.GetDataType();
}

Field::Field(const char* pValue, TmDataType emDataType)
: mDataType(emDataType)
{
	if (NULL == pValue)
	{
		mpValue = NULL;
		return;
	}
	
	mpValue = new char[strlen(pValue) + 1];
	if (NULL == mpValue)
	{
		return;
	}
	strcpy(mpValue, pValue);
}

Field::~Field()
{
	if (mpValue)
	{
		delete []mpValue;
		mpValue = NULL;
	}
}

void Field::SetValue(const char* pValue)
{
	if (NULL != mpValue)
	{
		delete [] mpValue;
		mpValue = NULL;
	}

	if (NULL != pValue)
	{
		mpValue = new char [strlen(pValue) + 1];
		if (NULL != mpValue)
		{
			strcpy(mpValue, pValue);
		}
	}
	else
	{
		mpValue = NULL;
	}
}

void Field::SetValue(const char* pValue, int len)
{
	if (NULL != mpValue)
	{
		delete []mpValue;
		mpValue = NULL;
	}
	if (NULL != pValue && len > 0)
	{
		mpValue = new char [len + 1];
		mpValue[len] = '\0';
		mValueLen = len;
		//strncpy(mpValue, pValue, len);
		memcpy(mpValue, pValue, len);
	}
	else
	{
		mValueLen = 0;
		mpValue = NULL;
	}
}
