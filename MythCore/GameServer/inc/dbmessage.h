#ifndef __DBMESSAGE_H__
#define __DBMESSAGE_H__
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
	char				mSqlBuffer[65000];		// ���ݻ�����
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
	short		mDataOffset;			// �Ѿ���ȡ������
	short		mSqlLenth;				// ����������
};

class CDBResponse : public CDBResponseHeader
{
	char		mSqlBuffer[65000];		// ���ݻ�����
};
#endif