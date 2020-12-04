#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "servercommon.h"
#include "logmanager.h"
#include "logintype.h"
#include "ranktype.h"
#include "messagefactory.h"
// IM表示内部消息（internal message的简写)
enum EmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// 玩家本地日志请求
	IM_REQUEST_PLAT_LOG					= 2,			// 平台日志请求
	IM_REQUEST_PLAT_WEB					= 3,			// 平台Web请求
	IM_RESPONSE_PLAT_WEB				= 4,			// 平台web回应
	IM_REQUEST_UPDATE_RANK				= 5,			// 更新排行榜请求
	IM_RESPONSE_UPDATE_RANK				= 6,			// 更新排行榜回应
	IM_REQUEST_GET_RANK_INFO			= 7,			// 得到排行信息请求
	IM_RESPONSE_GET_RANK_INFO			= 8,			// 得到排行信息的回应
	IM_REQUEST_PLAYER_LOGIN				= 9,			// 玩家登陆
	IM_REQUEST_DB_SQL					= 10,			// DB sql语句直接请求
	IM_REQUEST_DB_MSG					= 11,			// DB 消息方式的请求
};

#define RETURN_DATA_LENGTH 1024

// 为了和服务器与服务器之间，服务器与客户端之间的消息相区分
// 内部消息都已CIM开头
class CInternalMsg
{
public:
	CInternalMsg(){}
	~CInternalMsg(){}

public:
	int			getMsgID(){return mMsgID;}
	void		setMsgID(int uMsgID){mMsgID = uMsgID;}

protected:
	int			mMsgID;										// 消息ID
};

// 本地日志请求
class CIMLocalLogRequest : public CInternalMsg
{
public:
	byte		mLogType;									// 日志类型		
	char		mLogContent[MAX_LOG_BUFFER_NUM];			// 日志内容
#ifdef __DEBUG__
	char		mDebugName[MAX_DEBUG_LOG_NAME];				// 调试日志名字
#endif
};

// 平台日志请求
class CIMPlatLogRequest : public CInternalMsg
{
public:
	char		mContent[PLAT_LOG_CONTENT_LEN];			// 日志内容
};


// 平台Web请求
class CIMPlatWebRequest : public CInternalMsg
{
public:
	int			mHttpType;								// Http类型
	char		mURL[WEB_URL_LEN];						// URL
	char		mPostData[WEB_POST_DATA_LEN];			// Post数据的长度
	bool		mNeedCallBack;							// 是否需要回调
};

// 平台web请求回应
class CIMPlatWebResponse : public CInternalMsg
{
public:
	int			mHttpType;								// Http类型
	char		mReturnData[RETURN_DATA_LENGTH];		// Post数据的长度
};

// 更新排行榜请求
class CIMUpdateRankRequest : public CInternalMsg
{
public:
	byte		mRankType;							// 排行榜类型
	uint		mRoleID;							// 玩家角色ID
	int			mRoleObjID;							// 玩家实体ID
	int			mValue;								// 排行榜值
	time_t		mTime;								// 时间
};

// 更新排行榜回应
class CIMUpdateRankResponse : public CInternalMsg
{
public:
	uint		mRoleID;							// 玩家角色ID
	int			mRoleObjID;							// 玩家实体ID
};

// 得到排行榜的请求
class CIMGetRankInfoRequest : public CInternalMsg
{
public:
	byte		mRankType;							// 排行榜类型
	byte		mStartPlace;						// 开始名次
	byte		mEndPlace;							// 结束名次
	uint		mSelfRoleID;						// 自己的角色ID,如果mRoleID大于0，表示某个玩家取排行信息，如果mRoleID为0表示系统取排行信息
	uint		mSelfObjID;							// 自己的Obj ID
};

class CIMGetRankInfoResponse : public CInternalMsg
{
public:
	byte	mRankType;							// 排行榜类型
	uint	mRankRoleID[MAX_RANK_SHOW_NUM];		// 排行的角色ID
	int		mRankValue[MAX_RANK_SHOW_NUM];		// 排行值
	int		mCount;								// 排行数量
	
	uint	mSelfRoleID;						// 自己的角色ID,如果mRoleID大于0，表示某个玩家取排行信息，如果mRoleID为0表示系统取排行信息
	uint	mSelfObjID;							// 自己的Obj ID
	short	mSelfRankPlace;						// 排行名次
	int		mSelfRankValue;						// 排行值
};

class CIMPlayerLoginRequest : public CInternalMsg
{
public:
	time_t	mSocketTime;						// socket时间
	short	mSocketIndex;						// socket索引
	void*	mMsgQueue;							// 消息队列指针
	int		mJobID;								// 那个Job发过来的
};

class CIMDBMsgBase : public CInternalMsg
{
public:
	CDBRequestHeader	mHeader;					// 请求头
};

class CIMDBSqlRequest : public CIMDBMsgBase
{
public:
	byte				mSql[1024];					// sql语句
};

class CIMDBMsgRequest : public CIMDBMsgBase
{
public:
	Message*			mSqlMsg;					// 存储结构
};

class CIMDBMsgResponse : public CInternalMsg
{
public:
	CDBResponseHeader	mHeader;					// 回应头
	byte*				mMsgBuffer;					// 消息Buffer
};
#endif