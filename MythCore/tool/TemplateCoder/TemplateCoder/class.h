#ifndef __CLASS_H__
#define __CLASS_H__

#include "commontype.h"
#include "variable.h"
class CClass
{
public:
	CClass()
	{
		mOutClass = NULL;
		mCurClassIndex = 0;
		mName[0] = '\0';
		mParentName[0] = '\0';
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

		for (int i = 0; i < mSubClassList.size(); ++i)
		{
			delete mSubClassList[i];
		}
	}
public:
	typedef vector<CClass*>		SUB_CLASS_VECTOR;
	typedef vector<CVariable*>		VARIABLE_VECTOR;

public:
	SUB_CLASS_VECTOR&		getSubClassList(){ return mSubClassList; }
	VARIABLE_VECTOR&		getVariableList(){ return mVariableList; }

	CClass* getOutClass() const { return mOutClass; }
	void setOutClass(CClass* val) { mOutClass = val; }

	const char* getName() { return mName; }
	void setName(const char* pName)
	{
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName)-1);
	}

	int getCurClassIndex() const { return mCurClassIndex; }
	void setCurClassIndex(int val) { mCurClassIndex = val; }

	const char* getParentName(){return mParentName;}
	void setParentName(const char* pParentName)
	{
		if (NULL == pParentName)
		{
			return;
		}
		strncpy(mParentName, pParentName, sizeof(mParentName) - 1);
	}
private:
	SUB_CLASS_VECTOR		mSubClassList;					// �����б�
	VARIABLE_VECTOR			mVariableList;					// �����б�
	CClass*					mOutClass;						// �����
	char					mName[CLASS_NAME_LENGTH];		// ����
	int						mCurClassIndex;					// ��ǰ�����������
	char					mParentName[CLASS_NAME_LENGTH];	// ���������
};

#endif