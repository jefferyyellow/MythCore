#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "servercommon.h"
#include "logmanager.h"
#include "logintype.h"
// IM表示内部消息（internal message的简写)
enum EmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// 玩家本地日志要求
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
	int			getMsgID(){return mMsgID;}
	void		setMsgID(int uMsgID){mMsgID = uMsgID;}

protected:
	int			mMsgID;
};


class CIMLocalLogRequest : public CInternalMsg
{
public:
	byte		mLogType;									// 日志类型		
	char		mLogContent[MAX_LOG_BUFFER_NUM];			// 日志内容
#ifdef __DEBUG__
	char		mDebugName[MAX_DEBUG_LOG_NAME];				// 调试日志名字
#endif
};
// 玩家登陆内部消息基类
class CIMPlayerLoginMsg : public CInternalMsg
{
public:
	short			mSocketIndex;							// socket索引
	unsigned int	mAccountID;								// 账号ID
	short			mChannelID;								// 渠道
	short			mServerID;								// 服务器ID
};

class CIMPlayerLoginRequest : public CIMPlayerLoginMsg
{
public:
	char			mName[MAX_PLAYER_NAME_LEN];				// 名字
};

class CIMPlayerLoginResponse : public CIMPlayerLoginMsg
{
public:
	unsigned int	mRoleID;								// 角色ID
	char			mName[MAX_PLAYER_NAME_LEN];				// 名字
};

class CIMCreateRoleRequest : public CIMPlayerLoginMsg
{
public:
	char			mRoleName[MAX_PLAYER_NAME_LEN];			// 角色名
};


class CIMCreateRoleResponse : public CIMPlayerLoginMsg
{
public:
	unsigned int	mRoleID;								// 角色ID
	char			mRoleName[MAX_PLAYER_NAME_LEN];			// 角色名
};

class CIMEnterSceneRequest : public CIMPlayerLoginMsg
{
public:
	unsigned int	mRoleID;								// 角色ID
	unsigned int	mPlayerEntityID;						// 角色的实体ID
};

class CIMEnterSceneResponse : public CIMPlayerLoginMsg
{
public:
	unsigned int	mRoleID;								// 角色ID
	unsigned int	mPlayerEntityID;						// 角色的实体ID
};
#endif