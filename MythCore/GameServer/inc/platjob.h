#ifndef __PLATJOB_H__
#define __PLATJOB_H__
#include "job.h"
using namespace Myth;
#include "selectmodel.h"
#include "singleton.h"
#include "curlhandle.h"
#define SERVER_SOCKET_BUFF_SIZE		4096
#define SERVER_SOCKET_CACHE_SIZE	8192
void	sendPlatWebRequest(const char* pURL, const char* pData, EmHttpType eHttpType, bool bNeedBack);

class CURLHandle;
class CAsyncRedis;
class CPlatJob : public CJob < 100, 100 >, public CSingleton<CPlatJob>
{
public:
	friend class CSingleton < CPlatJob > ;

public:
	CPlatJob()
	{
		init();
	}
	~CPlatJob()
	{
	}
	
	void	init()
	{

	}
	bool	initAll(const char* pRedisIP, int nRedisPort, int nSocketNum, int nListenPort);

public:
	virtual void doing(int uParam);

public:
	/// 初始化socket
	bool	initSocket(int nSocketNum, int nListenPort);
	/// 接收消息
	void		receiveMessage();
	/// 清除socket
	void		clearSocket(int nTcpIndex, CTcpSocket* pSocket);
	/// 收到消息
	void		onReceiveMessage(CTcpSocket* pSocket);
	/// 处理平台日志
	void		onPlatLogRequest(CInternalMsg* pIMMsg);
	/// 处理Web请求
	void		onPlatWebRequest(CInternalMsg* pIMMsg);

public:
	static		void RedisConnectHandle(const struct redisAsyncContext*, int status);
	static		void RedisDisconnectHandle(const struct redisAsyncContext*, int status);
	static		void CommandCallBack(redisAsyncContext*, void *reply, void *privdata);

private:
	CSelectModel*	mSelectModel;	// Select模型
	CTcpSocket*		mTcpSocket;		// Socket
	int				mSocketNum;		// socket的数量

	CURLHandle*		mURLHandle;		// Web处理
	CAsyncRedis*	mAsyncRedis;	// 异步Redis
};
#endif