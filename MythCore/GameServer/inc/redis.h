#ifndef __REDIS_H__
#define __REDIS_H__
#include "hiredis.h"
class CRedisReply
{
public:
	CRedisReply()
	{
		init();
	}

	~CRedisReply()
	{
		if (NULL != mReply)
		{
			freeReplyObject(mReply);
		}
	}

	void	init()
	{

	}

public:
	redisReply*		getReply(){ return mReply; }
	void			setReply(redisReply* pReply){ mReply = pReply; }

	int				getType()
	{
		if (NULL == mReply)
		{
			return 0;
		}
		return mReply->type;
	}
	long long		getInteger()
	{
		if (NULL == mReply)
		{
			return 0;
		}
		return mReply->integer;
	}
	int				getStrLen()
	{
		if (NULL == mReply)
		{
			return 0;
		}
		return mReply->len;
	}
	char*			getStr()
	{
		if (NULL == mReply)
		{
			return NULL;
		}
		return mReply->str;
	}

	size_t			getElementNum()
	{
		if (NULL == mReply)
		{
			return NULL;
		}
		return mReply->elements;
	}

	redisReply**	getElement()
	{
		if (NULL == mReply)
		{
			return NULL;
		}
		return mReply->element;
	}
private:
	redisReply*		mReply;
};

class CRedis
{
public:
	CRedis()
	{
		mpContext = NULL;
	}
	~CRedis()
	{
		if (NULL != mpContext)
		{
			redisFree(mpContext);
		}
	}

public:
	// 连接redis服务器
	int		ConnectRedisServer(const char *ip, int port, int nTime);
	int		Command(CRedisReply& rRedisReplay, const char *format, ...);

private:
	redisContext* mpContext;
};


// 异步的还依赖LibEvent库，先不处理
//typedef void CommandHandle(void* privdata);
//struct redisAsyncContext;
//struct event_base;
//class CAsyncRedis
//{
//public:
//	CAsyncRedis();
//	~CAsyncRedis();
//	void	Clear();
//
//	// 连接redis服务器
//	int		ConnectRedisServer(const char *ip, int port);
//	void	DoEvent();
//	int		Command(void *privdata, const char *format, ...);
//
//public:
//	void	SetCommandHandle(CommandHandle* pCommandHandle){ mCommnadHandle = pCommandHandle; }
//
//public:
//	static void	OnConnectCallBack(const redisAsyncContext*, int status);
//	static void	OnDisconnectCallBack(const redisAsyncContext*, int status);
//	static void OnCommandCallBack(redisAsyncContext* pContext, void* reply, void* privdata);
//
//private:
//	redisAsyncContext*		mpAsyncContext;
//	event_base*				mpEventBase;
//	bool					mConnected;
//	CommandHandle*			mCommnadHandle;
//};
#endif