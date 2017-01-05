#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "commontype.h"
#include "logmanager.h"
// IM��ʾ�ڲ���Ϣ��internal message�ļ�д)
enum TmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// ��ұ�����־Ҫ��
	IM_REQUEST_PLAYER_LOGIN				= 2,			// ��ҵ�½��Ϣ
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
};



#endif