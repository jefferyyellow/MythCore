
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

#define		MAX_STR_LEN			64				// 最大服务器名称字符串长度
#define		MAX_SERVER_NUM	12				// 最大服务器组数
#define     MAX_STR_NUM  256
#define    SUCCESS   0
#define    UNKNOW_ERROR 1

class CMessage
{
public:
	int ID;
	int Size; 
};

// 重启服务器请求
class CMessageRestartServerRequest : public CMessage
{
public:
	int SelectServer[MAX_SERVER_NUM];
};

// 重启服务器回应
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

// 得到服务器日期及时间请求
class CMessageGetServerDateTimeRequest : public CMessage
{

};

// 得到服务器日期及时间回应
class CMessageGetServerDateTimeResponse : public CMessage
{
public:
    int DateTime;
};

// 设置服务器日期及时间的请求
class CMessageSetServerDateTimeRequest : public CMessage
{
public:
	int DateTime;					// 要设置的时间
	int	SelectServer[MAX_SERVER_NUM];					// 重启的服务器ID
};

// 设置服务器日期及时间的回应
class CMessageSetServerDateTimeResponse : public CMessage
{
public:
	int DateTime;					// 服务器的时间
};

// 停止服务器请求
class CMessageStopServerRequest : public CMessage
{
public:
	int SelectServer[MAX_SERVER_NUM];
};

// 停止服务器回应
class CMessageStopServerResponse : public CMessage
{
public:
	int Result; 
};

// 编译服务器完成通知
class CMessageCompileCompleteNotify : public CMessage
{
	//
};

// 请求服务器名称的请求
class CMessageGetServerNameRequest : public CMessage
{
	int ServerNum;
};

// 请求服务器名称的回应
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
