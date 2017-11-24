#ifndef __DBMESSAGE_H__
#define __DBMESSAGE_H__
class	CDBRequestHeader
{
public:
	int			mPlayerID;				// 玩家ID
	int			mParam1;				// 参数1
	int			mParam2;				// 参数2
	short		mSessionType;			// 会话类型
	short		mSqlLenth;				// 缓冲区长度

};

class CDBRequest : public CDBRequestHeader
{
public:
	char				mSqlBuffer[65000];		// 数据缓冲区
};

class CDBResponseHeader
{
public:
	int			mPlayerID;				// 玩家ID
	int			mResult;				// 结果
	int			mParam1;				// 参数1
	int			mParam2;				// 参数2
	short		mSessionType;			// 会话ID
	short		mRowNum;				// 行数
	short		mColNum;				// 列数
	short		mDataOffset;			// 已经读取的数据
	short		mSqlLenth;				// 缓冲区长度
};

class CDBResponse : public CDBResponseHeader
{
	char		mSqlBuffer[65000];		// 数据缓冲区
};
#endif