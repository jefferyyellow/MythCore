#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "commontype.h"
enum TmInternalMsgID
{
	ID_REQUEST_PLAYER_LOGIN_MSG			= 1,			// ��ҵ�½��Ϣ
};


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

class CPlayerLoginRequest : public CInternalMsg
{
private:
	char		mName[32];			// ����
	uint16		mChannelID;			// ����
	uint16		mWorldID;			// ������ID
};

#endif