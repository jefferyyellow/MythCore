#include "redis.h"
#include <WinSock2.h>
int CRedis::ConnectRedisServer(const char *pIp, int nPort, int nMsTime)
{
	if (NULL == pIp)
	{
		return -1;
	}
	if (nMsTime == 0)
	{
		mpContext = redisConnect(pIp, nPort);
	}
	else
	{
		timeval tTimeValue;
		if (nMsTime > 1000)
		{
			tTimeValue.tv_sec = nMsTime / 1000;
		}
		tTimeValue.tv_usec = nMsTime % 1000 * 1000;
		mpContext = redisConnectWithTimeout(pIp, nPort, tTimeValue);
	}

	return 0;
}

int CRedis::Command(CRedisReply& rRedisReplay, const char *format, ...)
{
	if (NULL == format || NULL == mpContext)
	{
		return -1;
	}

	va_list ap;
	void *reply = NULL;
	va_start(ap, format);

	redisReply* pReply = (redisReply*)redisvCommand(mpContext, format, ap);
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

////-------------------------------------------------------------------------------------------------------------------------
//CAsyncRedis::CAsyncRedis()
//{
//	mpAsyncContext = NULL;
//	mpEventBase = event_base_new();
//	mConnected = false;
//}
//
//CAsyncRedis::~CAsyncRedis()
//{
//	Clear();
//}
//
//void CAsyncRedis::Clear()
//{
//	if (NULL != mpAsyncContext)
//	{
//		redisAsyncDisconnect(mpAsyncContext);
//	}
//	if (NULL != mpEventBase)
//	{
//		event_base_free(mpEventBase);
//	}
//}
//
//int CAsyncRedis::ConnectRedisServer(const char *ip, int port)
//{
//	mpAsyncContext = redisAsyncConnect(ip, port);
//	if (NULL == mpAsyncContext)
//	{
//		Clear();
//		return -1;
//	}
//	if (mpAsyncContext->err)
//	{
//		Clear();
//		return -2;
//	}
//
//	int tRet = redisLibeventAttach(mpAsyncContext, mpEventBase);
//	if (tRet != REDIS_OK)
//	{
//		Clear();
//		return -3;
//	}
//
//
//	tRet = redisAsyncSetConnectCallback(mpAsyncContext, OnConnectCallBack);
//	if (tRet == REDIS_ERR)
//	{
//		return -4;
//	}
//	tRet = redisAsyncSetDisconnectCallback(mpAsyncContext, OnDisconnectCallBack);
//	if (tRet == REDIS_ERR)
//	{
//		return -5;
//	}
//	return 0;
//}
//
//void CAsyncRedis::DoEvent()
//{
//	if (NULL != mpEventBase)
//	{
//		event_base_loop(mpEventBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);
//	}
//}
//
//
//int CAsyncRedis::Command(void *privdata, const char *format, ...)
//{
//	if (NULL == format || NULL == mpAsyncContext || !mConnected)
//	{
//		return -1;
//	}
//
//	va_list ap;
//	void *reply = NULL;
//	va_start(ap, format);
//
//	int tRet = redisvAsyncCommand(mpAsyncContext, OnCommandCallBack, privdata, format, ap);
//	va_end(ap);
//
//	return tRet;
//}
//
//void CAsyncRedis::OnConnectCallBack(const struct redisAsyncContext*, int status)
//{
//	if (REDIS_OK != status)
//	{
//		return;
//	}
//	CAsyncRedis& rAsyncRedis = CSceneLogic::GetSingletonPtr()->GetAsyncRedis();
//	rAsyncRedis.mConnected = true;
//}
//
//void CAsyncRedis::OnDisconnectCallBack(const struct redisAsyncContext*, int status)
//{
//	if (REDIS_OK != status)
//	{
//		return;
//	}
//	CAsyncRedis& rAsyncRedis = CSceneLogic::GetSingletonPtr()->GetAsyncRedis();
//	rAsyncRedis.mConnected = false;
//}
//
//void CAsyncRedis::OnCommandCallBack(redisAsyncContext* pContext, void* reply, void* privdata)
//{
//	CAsyncRedis& rAsyncRedis = CSceneLogic::GetSingletonPtr()->GetAsyncRedis();
//	if (NULL != rAsyncRedis.mCommnadHandle)
//	{
//		rAsyncRedis.mCommnadHandle(privdata);
//	}
//}