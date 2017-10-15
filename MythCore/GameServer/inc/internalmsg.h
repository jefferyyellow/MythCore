#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "commontype.h"
#include "logmanager.h"
#include "logintype.h"
// IM表示内部消息（internal message的简写)
enum EmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// 玩家本地日志要求
	IM_REQUEST_PLAYER_LOGIN				= 2,			// 玩家登陆消息
	IM_RESPONSE_PLAYER_LOGIN			= 3,			// 玩家登陆回应
	IM_REQUEST_CREATE_ROLE				= 4,			// 玩家创建角色请求
	IM_RESPONSE_CREATE_ROLE				= 5,			// 玩家创建角色回应
	IM_REQUEST_ENTER_SCENE				= 6,			// 玩家进入场景的请求
	IM_RESPONSE_ENTER_SCENE				= 7,			// 玩家进入场景的回应
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
	uint8		mLogType;									// 日志类型		
	char		mLogContent[MAX_LOG_BUFFER_NUM];			// 日志内容
#ifdef __DEBUG__
	char		mDebugName[MAX_DEBUG_LOG_NAME];				// 调试日志名字
#endif
};
// 玩家登陆内部消息基类
class CIMPlayerLoginMsg : public CInternalMsg
{
public:
	uint16			mSocketIndex;							// socket索引
	uint32			mAccountID;								// 账号ID
	uint16			mChannelID;								// 渠道
	uint16			mServerID;								// 服务器ID
};

class CIMPlayerLoginRequest : public CIMPlayerLoginMsg
{
public:
	char			mName[MAX_PLAYER_NAME_LEN];				// 名字
};

class CIMPlayerLoginResponse : public CIMPlayerLoginMsg
{
public:
	uint32			mRoleID;								// 角色ID
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
	uint32			mRoleID;								// 角色ID
	char			mRoleName[MAX_PLAYER_NAME_LEN];			// 角色名
};

class CIMEnterSceneRequest : public CIMPlayerLoginMsg
{
public:
	uint32			mRoleID;								// 角色ID
	uint32			mPlayerEntityID;						// 角色的实体ID
};

class CIMEnterSceneResponse : public CIMPlayerLoginMsg
{
public:
	uint32			mRoleID;								// 角色ID
	uint32			mPlayerEntityID;						// 角色的实体ID
};
#endif