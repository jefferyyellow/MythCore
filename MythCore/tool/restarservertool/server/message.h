
#ifndef __MESSAGE_H__
#define __MESSAGE_H__ 

#include <string.h>

enum TmMessageID
{
	emC2SRequest_RestartServer			= 128,
	emS2CResponse_RestartServer			= 129,
	emC2SRequest_GetServerDateTime		= 130,
	emS2CResponse_GetServerDateTime		= 131,
	emC2SRequest_SetServerDateTime		= 132,
	emS2CResponse_SetServerDateTime		= 133,
	emC2SRequest_StopServer				= 134,
	emS2CResponse_StopServer			= 135,
	emS2CNotify_CompileComplete	= 136,
	emC2SRequest_GetServerName		= 137,
	emS2CResponse_GetServerName		= 138,
};

#define		MAX_STR_LEN			64				// �������������ַ�������
#define		MAX_SERVER_NUM	12				// ������������
#define     MAX_STR_NUM  256
#define    SUCCESS   0
#define    UNKNOW_ERROR 1

class CMessage
{
public:
	int ID;
	int Size; 
};

// ��������������
class CMessageRestartServerRequest : public CMessage
{
public:
	int SelectServer[MAX_SERVER_NUM];
};

// ������������Ӧ
class CMessageRestartServerResponse : public CMessage
{
public:
	int Result;
	char ServerName[MAX_STR_NUM];
public:
	void Initialize()
	{
		Result = 0;
		memset(ServerName, 0, sizeof(ServerName));
	}
	CMessageRestartServerResponse()
	{
		Initialize();
	}
};

// �õ����������ڼ�ʱ������
class CMessageGetServerDateTimeRequest : public CMessage
{

};

// �õ����������ڼ�ʱ���Ӧ
class CMessageGetServerDateTimeResponse : public CMessage
{
public:
    int DateTime;
};

// ���÷��������ڼ�ʱ�������
class CMessageSetServerDateTimeRequest : public CMessage
{
public:
	int DateTime;					// Ҫ���õ�ʱ��
	int	SelectServer[MAX_SERVER_NUM];					// �����ķ�����ID
};

// ���÷��������ڼ�ʱ��Ļ�Ӧ
class CMessageSetServerDateTimeResponse : public CMessage
{
public:
	int DateTime;					// ��������ʱ��
};

// ֹͣ����������
class CMessageStopServerRequest : public CMessage
{
public:
	int SelectServer[MAX_SERVER_NUM];
};

// ֹͣ��������Ӧ
class CMessageStopServerResponse : public CMessage
{
public:
	int Result; 
};

// ������������֪ͨ
class CMessageCompileCompleteNotify : public CMessage
{
	//
};

// ������������Ƶ�����
class CMessageGetServerNameRequest : public CMessage
{
	int ServerNum;
};

// ������������ƵĻ�Ӧ
class CMessageGetServerNameResponse : public CMessage
{
public:
	char mServerName[MAX_SERVER_NUM][MAX_STR_LEN];

public:
	void Initialize()
	{
		memset( mServerName, 0, sizeof(mServerName) );
	}
	CMessageGetServerNameResponse()
	{
		Initialize();
	}
};

#endif
