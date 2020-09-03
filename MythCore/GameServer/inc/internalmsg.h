#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "servercommon.h"
#include "logmanager.h"
#include "logintype.h"
// IM表示内部消息（internal message的简写)
enum EmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// 玩家本地日志请求
	IM_REQUEST_PLAT_LOG					= 2,			// 平台日志请求
	IM_REQUEST_PLAT_WEB					= 3,			// 平台Web请求
	IM_RESPONSE_PLAT_WEB				= 4,			// 平台web回应
	IM_REQUEST_UPDATE_RANK				= 5,			// 更新排行榜请求
	IM_RESPONSE_UPDATE_RANK				= 6,			// 更新排行榜回应
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
	int			mRankType;							// 排行榜类型
	uint		mRoleID;							// 玩家角色ID
	int			mValue;								// 排行榜值
	time_t		mTime;								// 时间
};

// 更新排行榜回应
class CIMUpdateRankResponse : public CInternalMsg
{
public:
	int			mRankType;							// 排行榜类型
	uint		mRoleID;							// 玩家角色ID
};

#endif