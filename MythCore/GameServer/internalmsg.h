#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "commontype.h"
// IM表示内部消息（internal message的简写)
enum TmInternalMsgID
{
	IM_REQUEST_PLAYER_LOGIN				= 1,			// 玩家登陆消息
};

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


class CIMPlayerLoginRequest : public CInternalMsg
{
public:
	char		mName[32];			// 名字
	uint16		mChannelID;			// 渠道
	uint16		mWorldID;			// 服务器ID
};

#endif