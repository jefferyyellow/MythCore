#include "redis.h"
#include "event.h"
#include "async.h"
#include "adapters/libevent.h"
int CRedis::connectRedisServer(const char *pIp, int nPort, int nMsTime)
{
	if (NULL == pIp)
	{
		return -1;
	}
	if (nMsTime == 0)
	{
		mContext = redisConnect(pIp, nPort);
	}
	else
	{
		timeval tTimeValue;
		if (nMsTime > 1000)
		{
			tTimeValue.tv_sec = nMsTime / 1000;
		}
		tTimeValue.tv_usec = nMsTime % 1000 * 1000;
		mContext = redisConnectWithTimeout(pIp, nPort, tTimeValue);
	}

	return 0;
}

int CRedis::command(CRedisReply& rRedisReplay, const char *format, ...)
{
	if (NULL == format || NULL == mContext)
	{
		return -1;
	}

	va_list ap;
	void *reply = NULL;
	va_start(ap, format);

	redisReply* pReply = (redisReply*)redisvCommand(mContext, format, ap);
	rRedisReplay.setReply(pReply);
	va_end(ap);
	if (NULL == pReply)
	{
		return -1;
	}

	if (REDIS_REPLY_ERROR == pReply->type)
	{
		return -1;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------
CAsyncRedis::CAsyncRedis()
{
	mAsyncContext = NULL;
	mEventBase = event_base_new();
	mConnected = false;
}

CAsyncRedis::~CAsyncRedis()
{
	clear();
}

void CAsyncRedis::clear()
{
	if (NULL != mAsyncContext)
	{
		redisAsyncDisconnect(mAsyncContext);
	}
	if (NULL != mEventBase)
	{
		event_base_free(mEventBase);
	}
}

bool CAsyncRedis::connectRedisServer(const char *ip, int port, redisConnectCallback* pConnectHandle,
	redisDisconnectCallback* pDisconnectHanle, redisCallbackFn* pCommandHandle)
{
	mAsyncContext = redisAsyncConnect(ip, port);
	if (NULL == mAsyncContext)
	{
		return false;
	}
	if (mAsyncContext->err)
	{
		return false;
	}

	int tRet = redisLibeventAttach(mAsyncContext, mEventBase);
	if (tRet != REDIS_OK)
	{
		return false;
	}

	tRet = redisAsyncSetConnectCallback(mAsyncContext, pConnectHandle);
	if (tRet == REDIS_ERR)
	{
		return false;
	}
	tRet = redisAsyncSetDisconnectCallback(mAsyncContext, pDisconnectHanle);
	if (tRet == REDIS_ERR)
	{
		return false;
	}
	mCommandHandle = pCommandHandle;
	return true;
}

void CAsyncRedis::doEvent()
{
	if (NULL != mEventBase)
	{
		event_base_loop(mEventBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);
	}
}


int CAsyncRedis::command(void *privdata, const char *format, ...)
{
	if (NULL == format || NULL == mAsyncContext || !mConnected)
	{
		return -1;
	}

	va_list ap;
	void *reply = NULL;
	va_start(ap, format);

	int tRet = redisvAsyncCommand(mAsyncContext, mCommandHandle, privdata, format, ap);
	va_end(ap);

	return tRet;
}

char* CAsyncRedis::getErrStr()
{
	return mAsyncContext->errstr;
}