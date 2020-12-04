#ifndef __INTERNALMSG_H__
#define __INTERNALMSG_H__
#include "servercommon.h"
#include "logmanager.h"
#include "logintype.h"
#include "ranktype.h"
#include "messagefactory.h"
// IM��ʾ�ڲ���Ϣ��internal message�ļ�д)
enum EmInternalMsgID
{
	IM_REQUEST_LOCAL_LOG				= 1,			// ��ұ�����־����
	IM_REQUEST_PLAT_LOG					= 2,			// ƽ̨��־����
	IM_REQUEST_PLAT_WEB					= 3,			// ƽ̨Web����
	IM_RESPONSE_PLAT_WEB				= 4,			// ƽ̨web��Ӧ
	IM_REQUEST_UPDATE_RANK				= 5,			// �������а�����
	IM_RESPONSE_UPDATE_RANK				= 6,			// �������а��Ӧ
	IM_REQUEST_GET_RANK_INFO			= 7,			// �õ�������Ϣ����
	IM_RESPONSE_GET_RANK_INFO			= 8,			// �õ�������Ϣ�Ļ�Ӧ
	IM_REQUEST_PLAYER_LOGIN				= 9,			// ��ҵ�½
	IM_REQUEST_DB_SQL					= 10,			// DB sql���ֱ������
	IM_REQUEST_DB_MSG					= 11,			// DB ��Ϣ��ʽ������
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
	byte		mRankType;							// ���а�����
	uint		mRoleID;							// ��ҽ�ɫID
	int			mRoleObjID;							// ���ʵ��ID
	int			mValue;								// ���а�ֵ
	time_t		mTime;								// ʱ��
};

// �������а��Ӧ
class CIMUpdateRankResponse : public CInternalMsg
{
public:
	uint		mRoleID;							// ��ҽ�ɫID
	int			mRoleObjID;							// ���ʵ��ID
};

// �õ����а������
class CIMGetRankInfoRequest : public CInternalMsg
{
public:
	byte		mRankType;							// ���а�����
	byte		mStartPlace;						// ��ʼ����
	byte		mEndPlace;							// ��������
	uint		mSelfRoleID;						// �Լ��Ľ�ɫID,���mRoleID����0����ʾĳ�����ȡ������Ϣ�����mRoleIDΪ0��ʾϵͳȡ������Ϣ
	uint		mSelfObjID;							// �Լ���Obj ID
};

class CIMGetRankInfoResponse : public CInternalMsg
{
public:
	byte	mRankType;							// ���а�����
	uint	mRankRoleID[MAX_RANK_SHOW_NUM];		// ���еĽ�ɫID
	int		mRankValue[MAX_RANK_SHOW_NUM];		// ����ֵ
	int		mCount;								// ��������
	
	uint	mSelfRoleID;						// �Լ��Ľ�ɫID,���mRoleID����0����ʾĳ�����ȡ������Ϣ�����mRoleIDΪ0��ʾϵͳȡ������Ϣ
	uint	mSelfObjID;							// �Լ���Obj ID
	short	mSelfRankPlace;						// ��������
	int		mSelfRankValue;						// ����ֵ
};

class CIMPlayerLoginRequest : public CInternalMsg
{
public:
	time_t	mSocketTime;						// socketʱ��
	short	mSocketIndex;						// socket����
	void*	mMsgQueue;							// ��Ϣ����ָ��
	int		mJobID;								// �Ǹ�Job��������
};

class CIMDBMsgBase : public CInternalMsg
{
public:
	CDBRequestHeader	mHeader;					// ����ͷ
};

class CIMDBSqlRequest : public CIMDBMsgBase
{
public:
	byte				mSql[1024];					// sql���
};

class CIMDBMsgRequest : public CIMDBMsgBase
{
public:
	Message*			mSqlMsg;					// �洢�ṹ
};

class CIMDBMsgResponse : public CInternalMsg
{
public:
	CDBResponseHeader	mHeader;					// ��Ӧͷ
	byte*				mMsgBuffer;					// ��ϢBuffer
};
#endif