#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "commontype.h"
#include "logmanager.h"
// IM表示内部消息（internal message的简写)
enum TmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// 玩家本地日志要求
	IM_REQUEST_PLAYER_LOGIN				= 2,			// 玩家登陆消息
};

#define MAX_DEBUG_LOG_NAME			32

// 为了和服务器与服务器之间，服务器与客户端之间的消息相区分
// 内部消息都已CIM开头
class CInternalMsg
{
public:
	CInternalMsg(){}
	~CInternalMsg(){}

public:
	uint32		getMsgID(){return mMsgID;}
	void		setMsgID(uint32 uMsgID){mMsgID = uMsgID;}

protected:
	uint32		mMsgID;
};


class CIMLocalLogRequest : public CInternalMsg
{
public:
	uint8		mLogType;								// 日志类型		
	char		mLogContent[MAX_LOG_BUFFER_NUM];		// 日志内容
#ifdef __DEBUG__
	char		mDebugName[MAX_DEBUG_LOG_NAME];			// 调试日志名字
#endif
};

class CIMPlayerLoginRequest : public CInternalMsg
{
public:
	char		mName[32];								// 名字
	uint16		mChannelID;								// 渠道
	uint16		mWorldID;								// 服务器ID
};



#endif