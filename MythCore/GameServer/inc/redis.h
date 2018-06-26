#ifndef __REDIS_H__
#define __REDIS_H__
#include "hiredis.h"
#include "async.h"
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
        mReply = NULL;
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
		init();
	}
	~CRedis()
	{
		if (NULL != mContext)
		{
			redisFree(mContext);
		}
	}
	
	void init()
	{
        mContext = NULL;
	}

public:
	// 连接redis服务器
	int		connectRedisServer(const char *ip, int port, int nTime);
	int		command(CRedisReply& rRedisReplay, const char *format, ...);

private:
	redisContext* mContext;
};


struct redisAsyncContext;
struct event_base;
class CAsyncRedis
{
public:
	CAsyncRedis();
	~CAsyncRedis();
	void	clear();

	// 连接redis服务器
	bool	connectRedisServer(const char *ip, int port, redisConnectCallback pConnectHandle, 
		redisDisconnectCallback pDisconnectHanle, redisCallbackFn pCommandHandle);
	void	doEvent();
	int		command(void *privdata, const char *format, ...);
	char*	getErrStr();


	bool	getConnected(){return mConnected;}
	void	setConnected(bool bConnected){mConnected = bConnected;}
private:
	redisAsyncContext*			mAsyncContext;
	event_base*					mEventBase;
	bool						mConnected;
	redisCallbackFn*			mCommandHandle;
};
#endif