#ifndef __DBMESSAGE_H__
#define __DBMESSAGE_H__
#include "commontype.h"
class	CDBRequestHeader
{
public:
	int			mPlayerID;				// ���ID
	int			mParam1;				// ����1
	int			mParam2;				// ����2
	short		mSessionType;			// �Ự����
	short		mSqlLenth;				// ����������

};

class CDBRequest : public CDBRequestHeader
{
public:
	// ��չ��65000 * 2,mysql_real_escape_stringֱ�������������
	// mysql_real_escape_string�Ŀռ�Ҫ��length*2+1 
	byte		mSqlBuffer[130000];		// ���ݻ�����
};

class CDBResponseHeader
{
public:
	int			mPlayerID;				// ���ID
	int			mResult;				// ���
	int			mParam1;				// ����1
	int			mParam2;				// ����2
	short		mSessionType;			// �ỰID
	short		mRowNum;				// ����
	short		mColNum;				// ����
	short		mSqlLenth;				// ����������
};

class CDBResponse : public CDBResponseHeader
{
public:
	void		resetPoint()
	{
		mpLength = (unsigned long*)mSqlBuffer;
		mpValue = mSqlBuffer + (mRowNum * mColNum * sizeof(unsigned long));
	}
	byte		getByte()
	{
		byte tByte = atoi((const char*)mpValue);
		mpValue += (*mpLength + 1);
		++mpLength;
		return tByte;
	}
	short		getShort()
	{
		short tShort = atoi((const char*)mpValue);
		mpValue += (*mpLength + 1);
		++mpLength;
		return tShort;
	}
	int		getInt()
	{
		int tInt = atoi((const char*)mpValue);
		mpValue += (*mpLength + 1);
		++mpLength;
		return tInt;
	}

	sint64 getInt64()
	{
		sint64 tInt64 = atoll((const char*)mpValue);
		mpValue += (*mpLength + 1);
		++mpLength;
		return tInt64;
	} 

	void	getString(char* pDest, unsigned int nSize)
	{
		if (nSize <= *mpLength)
		{
			return;
		}

		strncpy(pDest, (const char*)mpValue, *mpLength);
		pDest[*mpLength] = '\0';
		mpValue += (*mpLength + 1);
		++mpLength;
	}

	/// ����
	byte* getValue()const{return mpValue;}
	/// ���ݳ���
	unsigned long getLength()const{return *mpLength;}
	/// ���ӳ���
	void next()
	{ 
		mpValue += (*mpLength + 1);
		++mpLength;
	}
public:
	byte				mSqlBuffer[65000];		// ���ݻ�����
	// ����һ��Ҫ�������,ֻ����CSceneJob�н����ֶε�ʱ�����Ҫ
	// ����ʱһ��Ҫ��sql����е��ֶ�˳��һ��
	byte*				mpValue;
	unsigned long*		mpLength;
};

enum EmSessionType
{
	emSessionType_None					= 0,
	emSessionType_AccountVerify			= 1,			// �˺�У��
	emSessionType_CreateRole			= 2,			// ������ɫ
	emSessionType_LoadPlayerInfo		= 3,			// ���������Ϣ
	emSessionType_LoadPlayerBaseProperty= 4,			// �����������
	emSessionType_SavePlayerInfo		= 5,			// �����������
	emSessionType_SavePlayerBaseProperty= 6,			// ������һ�������
};


#endif