#include "commontype.h"
class CVariableDefault
{
public:
	CVariableDefault()
	{
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
		memset(mName, 0, sizeof(mName));
		mArrayDimension = 0;
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

private:
	char 			mType[TYPE_NAME_LENGTH];	// ��������
	char			mName[TYPE_NAME_LENGTH];	// ������
	int				mArrayDimension;			// ����ά��
	char			mDefaultValue[TYPE_NAME_LENGTH];	// ��ʼ��ֵ����������˳�ʼ���Ͳ���Ĭ�ϳ�ʼ����
};