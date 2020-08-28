#ifndef __DBMESSAGE_H__
#define __DBMESSAGE_H__
#include "servercommon.h"
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
	// 扩展成65000 * 2,mysql_real_escape_string直接在这上面操作
	// mysql_real_escape_string的空间要求length*2+1 
	byte		mSqlBuffer[130000];		// 数据缓冲区
};

class CDBResponseHeader
{
public:
	int			mPlayerID;				// 玩家ID
	int			mSqlResult;				// SQL执行结果
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
	CDBResponse()
	{
		mpValue = NULL;
		mpLength = NULL;
	}
	~CDBResponse()
	{

	}
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

	unsigned int getUInt()
	{
		sint64 tInt64 = atoll((const char*)mpValue);
		mpValue += (*mpLength + 1);
		++mpLength;
		return (unsigned int)tInt64;
	}

	sint64 getInt64()
	{
		sint64 tInt64 = atoll((const char*)mpValue);
		mpValue += (*mpLength + 1);
		++mpLength;
		return tInt64;
	} 

	double	getDouble()
	{
		double tDouble = atof((const char*)mpValue);
		mpValue += (*mpLength + 1);
		++mpLength;
		return tDouble;
	}

	void	getString(char* pDest, unsigned int nSize)
	{
		if (nSize <= *mpLength)
		{
			strncpy(pDest, (const char*)mpValue, nSize - 1);
			pDest[nSize - 1] = '\0';
			mpValue += (*mpLength + 1);
			++mpLength;
			return;
		}

		strncpy(pDest, (const char*)mpValue, *mpLength);
		pDest[*mpLength] = '\0';
		mpValue += (*mpLength + 1);
		++mpLength;
	}

	/// 数据
	byte* getValue()const{return mpValue;}
	/// 数据长度
	unsigned long getLength()const{return *mpLength;}
	/// 增加长度
	void next()
	{ 
		mpValue += (*mpLength + 1);
		++mpLength;
	}
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
	emSessionType_SavePlayerInfo		= 5,			// 保存玩家属性
	emSessionType_SavePlayerBaseProperty= 6,			// 保存玩家基础属性
	emSessionType_LoadAllocateRoleId	= 7,			// 加载分配角色ID
	emSessionType_UpdateAllocateRoleId	= 8,			// 更新分配角色ID
	emSessionType_InsertRechargeCache	= 9,			// 插入充值缓存
	emSessionType_LoadRechargeCache		= 10,			// 加载充值缓存
	emSessionType_RechargeSuccess		= 11,			// 充值成功
	emSessionType_SavePlayerMail		= 12,			// 保存玩家邮件
	emSessionType_LoadPlayerMail		= 13,			// 加载玩家邮件
	emSessionType_LoadMaxMailID			= 14,			// 加载最大的邮件ID
	emSessionType_DeleteExpireMail		= 15,			// 删除过期邮件
	emSessionType_SaveGlobalMail		= 16,			// 保存全局邮件
	emSessionType_LoadGlobalMail		= 17,			// 加载全局邮件
	emSessionType_DeleteGlobalMail		= 18,			// 删除全局邮件
	emSessionType_LoadMaxGlobalMailID	= 19,			// 加载最大的全局邮件ID
	emSessionType_DeleteMail			= 20,			// 删除邮件
	emSessionType_UpdateMailStatus		= 21,			// 更新邮件状态
};


#endif