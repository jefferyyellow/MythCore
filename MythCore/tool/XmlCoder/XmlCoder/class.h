#ifndef __CLASS_H__
#define __CLASS_H__

#include "commontype.h"
#include "variable.h"
class CClass
{
public:
	CClass()
	{
		mName[0] = '\0';
		mNeedGeneral = false;
		mCount = 0;
	}
	~CClass()
	{
		clear();
	}

	void clear()
	{
		for (int i = 0; i < mVariableList.size(); ++i)
		{
			delete mVariableList[i];
		}
	}
public:
	typedef vector<CVariable*>		VARIABLE_VECTOR;

public:
	VARIABLE_VECTOR&		getVariableList(){ return mVariableList; }

	const char* getName() { return mName; }
	void setName(const char* pName) 
	{ 
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName) - 1);
	}

	bool getNeedGeneral() const { return mNeedGeneral; }
	void setNeedGeneral(bool nValue) { mNeedGeneral = nValue; }

	int getCount() const { return mCount; }
	void setCount(int nValue) { mCount = nValue; }

private:
	VARIABLE_VECTOR			mVariableList;					// 变量列表
	char					mName[CLASS_NAME_LENGTH];		// 类名
	bool					mNeedGeneral;					// 外部引用
	int						mCount;							// 数组维数
};

#endif