#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "commontype.h"
// IM��ʾ�ڲ���Ϣ��internal message�ļ�д)
enum TmInternalMsgID
{
	IM_REQUEST_PLAYER_LOGIN				= 1,			// ��ҵ�½��Ϣ
};

// Ϊ�˺ͷ������������֮�䣬��������ͻ���֮�����Ϣ������
// �ڲ���Ϣ����CIM��ͷ
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
	char		mName[32];			// ����
	uint16		mChannelID;			// ����
	uint16		mWorldID;			// ������ID
};

#endif