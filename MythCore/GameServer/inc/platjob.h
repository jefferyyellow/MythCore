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
class CPlatJob : public CJob, public CSingleton<CPlatJob>
{
public:
	friend class CSingleton < CPlatJob > ;

public:
	CPlatJob()
	{
		init();
		setJobType(emJobID_Plat);
	}
	~CPlatJob()
	{
		clear();
	}
	
	void	init()
	{

	}
	bool	initAll(const char* pRedisIP, int nRedisPort, int nSocketNum, int nListenPort);
	void	clear();
public:
	virtual void doing(int uParam);

public:
	/// ��ʼ��socket
	bool	initSocket(int nSocketNum, int nListenPort);
	/// ������Ϣ
	void		receiveMessage();
	/// ���socket
	void		clearSocket(int nTcpIndex, CTcpSocket* pSocket);
	/// �յ���Ϣ
	void		onReceiveMessage(CTcpSocket* pSocket);
	/// ����ƽ̨��־
	void		onPlatLogRequest(CInternalMsg* pIMMsg);
	/// ����Web����
	void		onPlatWebRequest(CInternalMsg* pIMMsg);

public:
	static		void RedisConnectHandle(const struct redisAsyncContext*, int status);
	static		void RedisDisconnectHandle(const struct redisAsyncContext*, int status);
	static		void CommandCallBack(redisAsyncContext*, void *reply, void *privdata);
	/// Web����Ļص�����
	static void	onURLCompleteCallBack(CURLSession* pURLSession);

private:
	CSelectModel*	mSelectModel;	// Selectģ��
	CTcpSocket*		mTcpSocket;		// Socket
	int				mSocketNum;		// socket������

	CURLHandle*		mURLHandle;		// Web����
	CAsyncRedis*	mAsyncRedis;	// �첽Redis
};
#endif