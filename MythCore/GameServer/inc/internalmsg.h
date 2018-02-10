#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "servercommon.h"
#include "logmanager.h"
#include "logintype.h"
// IM��ʾ�ڲ���Ϣ��internal message�ļ�д)
enum EmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// ��ұ�����־Ҫ��
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
	int			getMsgID(){return mMsgID;}
	void		setMsgID(int uMsgID){mMsgID = uMsgID;}

protected:
	int			mMsgID;
};


class CIMLocalLogRequest : public CInternalMsg
{
public:
	byte		mLogType;									// ��־����		
	char		mLogContent[MAX_LOG_BUFFER_NUM];			// ��־����
#ifdef __DEBUG__
	char		mDebugName[MAX_DEBUG_LOG_NAME];				// ������־����
#endif
};
// ��ҵ�½�ڲ���Ϣ����
class CIMPlayerLoginMsg : public CInternalMsg
{
public:
	short			mSocketIndex;							// socket����
	unsigned int	mAccountID;								// �˺�ID
	short			mChannelID;								// ����
	short			mServerID;								// ������ID
};

class CIMPlayerLoginRequest : public CIMPlayerLoginMsg
{
public:
	char			mName[MAX_PLAYER_NAME_LEN];				// ����
};

class CIMPlayerLoginResponse : public CIMPlayerLoginMsg
{
public:
	unsigned int	mRoleID;								// ��ɫID
	char			mName[MAX_PLAYER_NAME_LEN];				// ����
};

class CIMCreateRoleRequest : public CIMPlayerLoginMsg
{
public:
	char			mRoleName[MAX_PLAYER_NAME_LEN];			// ��ɫ��
};


class CIMCreateRoleResponse : public CIMPlayerLoginMsg
{
public:
	unsigned int	mRoleID;								// ��ɫID
	char			mRoleName[MAX_PLAYER_NAME_LEN];			// ��ɫ��
};

class CIMEnterSceneRequest : public CIMPlayerLoginMsg
{
public:
	unsigned int	mRoleID;								// ��ɫID
	unsigned int	mPlayerEntityID;						// ��ɫ��ʵ��ID
};

class CIMEnterSceneResponse : public CIMPlayerLoginMsg
{
public:
	unsigned int	mRoleID;								// ��ɫID
	unsigned int	mPlayerEntityID;						// ��ɫ��ʵ��ID
};
#endif