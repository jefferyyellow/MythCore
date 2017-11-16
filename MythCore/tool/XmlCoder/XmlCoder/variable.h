#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "commontype.h"

class CVariable
{
public:
	CVariable()
	{
		init();
	}
	~CVariable()
	{}

	void init()
	{
        mType[0] = '\0';
        mName[0] = '\0';
        mCount = 0;
	}

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

	int getCount() const { return mCount; }
	void setCount(int nValue) { mCount = nValue; }

private:
	char 			mType[CLASS_NAME_LENGTH];			// 变量类型
	char			mName[CLASS_NAME_LENGTH];			// 变量名
	int				mCount;								// 数组维数
};

#endif