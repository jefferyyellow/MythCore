#ifndef __CLASS_H__
#define __CLASS_H__

#include "commontype.h"
#include "variable.h"
class CPlusClass
{
public:
	CPlusClass()
	{
		mOutClass = NULL;
		mCurClassIndex = 0;
	}
	~CPlusClass()
	{
		clear();
	}

	void clear()
	{
		for (int i = 0; i < mVariableList.size(); ++i)
		{
			delete mVariableList[i];
		}

		mVariableList.clear();
		for (int i = 0; i < mSubClassList.size(); ++ i)
		{
			delete mSubClassList[i];
		}
		mSubClassList.clear();
	}
public:
	typedef vector<CPlusClass*>		SUB_CLASS_VECTOR;
	typedef vector<CVariable*>		VARIABLE_VECTOR;

public:
	SUB_CLASS_VECTOR&		getSubClassList(){ return mSubClassList; }
	VARIABLE_VECTOR&		getVariableList(){ return mVariableList; }

	CPlusClass* getOutClass() const { return mOutClass; }
	void setOutClass(CPlusClass* val) { mOutClass = val; }

	const char* getName() { return mName; }
	void setName(const char* pName) 
	{ 
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName) - 1);
	}

	int getCurClassIndex() const { return mCurClassIndex; }
	void setCurClassIndex(int val) { mCurClassIndex = val; }

private:
	SUB_CLASS_VECTOR		mSubClassList;					// �����б�
	VARIABLE_VECTOR			mVariableList;					// �����б�
	CPlusClass*				mOutClass;						// �����
	char					mName[CLASS_NAME_LENGTH];		// ����
	int						mCurClassIndex;					// ��ǰ�����������
};

#endif