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
};


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
	char		mURL[WEB_URL_LEN];						// URL
	char		mPostData[WEB_POST_DATA_LEN];			// Post数据的长度
	bool		mNeedCallBack;							// 是否需要回调
	int			mHttpType;								// Http类型
};
#endif