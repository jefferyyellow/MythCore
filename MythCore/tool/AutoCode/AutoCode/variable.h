#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "commontype.h"
class CVariableDefault
{
public:
	CVariableDefault()
	{
		mType[0] = '\0';
		mValue[0] = '\0';
	}
	~CVariableDefault(){}

	const char* getType() { return mType; }
	void setType(const char* pType) 
	{ 
		if (NULL == pType)
		{
			return;
		}

		strncpy(mType, pType, sizeof(mType) - 1);
	}

	const char* getValue() { return mValue; }
	void setValue(const char* pValue) 
	{
		if (NULL == pValue)
		{
			return;
		}
		strncpy(mValue, pValue, sizeof(mValue) - 1);
	}

private:
	char 			mType[TYPE_NAME_LENGTH];
	char			mValue[DEFAULT_VALUE_LENGTH];
};

class CVariable
{
public:
	CVariable()
	{
		mName[0] = '\0';
		mType[0] = '\0';
		mDefaultValue[0] = '\0';
		mArrayDimension = 0;
		mPublic = false;
	}
	~CVariable()
	{}

public:
	const char* getType() { return mType; }
	void setType(const char* pType)
	{
		if (NULL == pType)
		{
			return;
		}

		strncpy(mType, pType, sizeof(mType) - 1);
	}

	const char* getName() { return mName; }
	void setName(const char* pName) 
	{ 
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName) - 1);
	}

	int getArrayDimension() const { return mArrayDimension; }
	void setArrayDimension(int val) { mArrayDimension = val; }

	const char* getDefaultValue(){return mDefaultValue;}
	void setDefaultValue(const char* pDefaultValue)
	{
		if (NULL == pDefaultValue)
		{
			return;
		}
		strncpy(mDefaultValue, pDefaultValue, sizeof(mDefaultValue) - 1);
	}

	bool			checkDefaultValue()
	{
		if (mDefaultValue[0] != '\0')
		{
			return true;
		}
		return false;
	}

	char*			getArrayMaxLen(int nIndex)
	{
		if (nIndex < 0 || nIndex >= MAX_ARRAY_DIMENSION)
		{
			return NULL;
		}

		return mArrayMaxLen[nIndex];
	}

	void			setArrayMaxLen(int nIndex, const char* pArrayMaxLen)
	{
		if (nIndex < 0 || nIndex >= MAX_ARRAY_DIMENSION)
		{
			return;
		}

		strncpy(mArrayMaxLen[nIndex], pArrayMaxLen, TYPE_NAME_LENGTH);
	}

	bool getPublic() const { return mPublic; }
	void setPublic(bool nValue) { mPublic = nValue; }

private:
	char 			mType[TYPE_NAME_LENGTH];			// 变量类型
	char			mName[TYPE_NAME_LENGTH];			// 变量名
	int				mArrayDimension;					// 数组维数
	char			mArrayMaxLen[MAX_ARRAY_DIMENSION][TYPE_NAME_LENGTH];	// 不考虑3维数组
	char			mDefaultValue[TYPE_NAME_LENGTH];	// 初始化值（如果设置了初始化就不用默认初始化）
	bool			mPublic;
};

#endif