#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "commontype.h"
#include "logmanager.h"
// IM��ʾ�ڲ���Ϣ��internal message�ļ�д)
enum TmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// ��ұ�����־Ҫ��
	IM_REQUEST_PLAYER_LOGIN				= 2,			// ��ҵ�½��Ϣ
	IM_RESPONSE_PLAYER_LOGIN			= 3,			// ��ҵ�½��Ӧ
	IM_REQUEST_CREATE_ROLE				= 4,			// ��Ҵ�����ɫ����
	IM_RESPONSE_CREATE_ROLE				= 5,			// ��Ҵ�����ɫ��Ӧ
};

#define MAX_DEBUG_LOG_NAME			32

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


class CIMLocalLogRequest : public CInternalMsg
{
public:
	uint8		mLogType;								// ��־����		
	char		mLogContent[MAX_LOG_BUFFER_NUM];		// ��־����
#ifdef __DEBUG__
	char		mDebugName[MAX_DEBUG_LOG_NAME];			// ������־����
#endif
};

class CIMPlayerLoginRequest : public CInternalMsg
{
public:
	char		mName[32];								// ����
	uint16		mChannelID;								// ����
	uint16		mWorldID;								// ������ID
	uint		mSocketIndex;							// Socket����
	uint		mSocketTime;							// Socket����ʱ��
};

class CIMPlayerLoginResponse : public CInternalMsg
{
public:
	uint32		mAccountID;								// �˺�ID
	uint16		mChannelID;								// ����
	uint16		mWorldID;								// ������ID
	uint32		mRoleID;								// ��ɫID
	uint		mSocketIndex;							// Socket����
	uint		mSocketTime;							// Socket����ʱ��
};

class CIMCreateRoleRequest : public CInternalMsg
{
public:
	uint32		mAccountID;								// �˺�ID
	uint16		mChannelID;								// ����
	uint16		mWorldID;								// ������ID
	char		mRoleName[32];							// ��ɫ��
};


class CIMCreateRoleResponse : public CInternalMsg
{
public:
	uint32		mRoleID;								// ��ɫID
	uint32		mAccountID;								// �˺�ID
	uint16		mChannelID;								// ����
	uint16		mWorldID;								// ������ID
	uint32		mSocketIndex;							// Socket����
};
#endif