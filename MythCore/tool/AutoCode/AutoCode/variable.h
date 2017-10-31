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

	const char* getValue() { return mType; }
	void setValue(const char* pValue) {strncpy(mValue, pValue, sizeof(mValue) - 1);}

private:
	char 			mType[TYPE_NAME_LENGTH];
	char			mValue[DEFAULT_VALUE_LENGTH];
};

class CVariable
{
public:
	CVariable()
	{}
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
	char 			mType[TYPE_NAME_LENGTH];
	char			mName[TYPE_NAME_LENGTH];
};