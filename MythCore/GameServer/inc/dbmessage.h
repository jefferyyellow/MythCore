#ifndef __DBMESSAGE_H__
#define __DBMESSAGE_H__
#include "commontype.h"
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
	byte		mSqlBuffer[65000];		// 数据缓冲区
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
	short		mSqlLenth;				// 缓冲区长度
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
	/// 数据
	byte* getValue()const{return mpValue;}
	void setValue(byte* val){mpValue = val;}
	/// 数据长度
	unsigned long* getLength()const{return mpLength;}
	void setLength(unsigned long* val){mpLength = val;}
	/// 增加长度
	void increaseLength(){ ++mpLength;}
public:
	byte				mSqlBuffer[65000];		// 数据缓冲区
	// 下面一定要在最后面,只有在CSceneJob中解析字段的时候才需要
	// 解析时一定要和sql语句中的字段顺序一致
	byte*				mpValue;
	unsigned long*		mpLength;
};

enum EmSessionType
{
	emSessionType_None					= 0,
	emSessionType_AccountVerify			= 1,			// 账号校验
	emSessionType_CreateRole			= 2,			// 创建角色
	emSessionType_LoadPlayerInfo		= 3,			// 加载玩家信息
	emSessionType_LoadPlayerBaseProperty= 4,			// 加载玩家属性
};


#endif