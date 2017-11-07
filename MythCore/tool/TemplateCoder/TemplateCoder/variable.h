#ifndef __VARIABLE_H__
#define __VARIABLE_H__
#include "commontype.h"

class CVariable
{
public:
	CVariable()
	{
		mName[0] = '\0';
		mType[0] = '\0';
		mArrayDimension = 0;
		memset(mArrayMaxLen, 0, sizeof(mArrayMaxLen));
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

		strncpy(mType, pType, sizeof(mType)-1);
	}

	const char* getName() { return mName; }
	void setName(const char* pName)
	{
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName)-1);
	}

	int getArrayDimension() const { return mArrayDimension; }
	void setArrayDimension(int val) { mArrayDimension = val; }


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

private:
	char 			mType[TYPE_NAME_LENGTH];										// ��������
	char			mName[TYPE_NAME_LENGTH];										// ������
	int				mArrayDimension;												// ����ά��
	char			mArrayMaxLen[MAX_ARRAY_DIMENSION][TYPE_NAME_LENGTH];			// ������3ά����
};
#endif