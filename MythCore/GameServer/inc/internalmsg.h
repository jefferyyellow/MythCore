#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "servercommon.h"
#include "logmanager.h"
#include "logintype.h"
// IM��ʾ�ڲ���Ϣ��internal message�ļ�д)
enum EmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// ��ұ�����־����
	IM_REQUEST_PLAT_LOG					= 2,			// ƽ̨��־����
	IM_REQUEST_PLAT_WEB					= 3,			// ƽ̨Web����
	IM_RESPONSE_PLAT_WEB				= 4,			// ƽ̨web��Ӧ
	IM_REQUEST_UPDATE_RANK				= 5,			// �������а�����
	IM_RESPONSE_UPDATE_RANK				= 6,			// �������а��Ӧ
};

#define RETURN_DATA_LENGTH 1024

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
	int			mMsgID;										// ��ϢID
};

// ������־����
class CIMLocalLogRequest : public CInternalMsg
{
public:
	byte		mLogType;									// ��־����		
	char		mLogContent[MAX_LOG_BUFFER_NUM];			// ��־����
#ifdef __DEBUG__
	char		mDebugName[MAX_DEBUG_LOG_NAME];				// ������־����
#endif
};

// ƽ̨��־����
class CIMPlatLogRequest : public CInternalMsg
{
public:
	char		mContent[PLAT_LOG_CONTENT_LEN];			// ��־����
};


// ƽ̨Web����
class CIMPlatWebRequest : public CInternalMsg
{
public:
	int			mHttpType;								// Http����
	char		mURL[WEB_URL_LEN];						// URL
	char		mPostData[WEB_POST_DATA_LEN];			// Post���ݵĳ���
	bool		mNeedCallBack;							// �Ƿ���Ҫ�ص�
};

// ƽ̨web�����Ӧ
class CIMPlatWebResponse : public CInternalMsg
{
public:
	int			mHttpType;								// Http����
	char		mReturnData[RETURN_DATA_LENGTH];		// Post���ݵĳ���
};

// �������а�����
class CIMUpdateRankRequest : public CInternalMsg
{
public:
	int			mRankType;							// ���а�����
	uint		mRoleID;							// ��ҽ�ɫID
	int			mValue;								// ���а�ֵ
	time_t		mTime;								// ʱ��
};

// �������а��Ӧ
class CIMUpdateRankResponse : public CInternalMsg
{
public:
	int			mRankType;							// ���а�����
	uint		mRoleID;							// ��ҽ�ɫID
};

#endif