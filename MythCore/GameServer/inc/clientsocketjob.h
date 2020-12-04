#ifndef __CLIENTSOCKETJOB_H__
#define __CLIENTSOCKETJOB_H__
#include "commontype.h"
#include "servercommon.h"
#include "job.h"
#include "tcpsocket.h"
#include "selectmodel.h"
#include "bytestream.h"
using namespace Myth;

class CPlayerMsgQueue
{
public:
	CPlayerMsgQueue();
	~CPlayerMsgQueue();

public:
	void	init(int nReceiveSize, int nSendSize);

public:
	/// 接收消息列表
	CByteStream	mReceiveStream;
	/// 发送消息列表
	CByteStream	mSendStream;
};

class CClientSocketJob : public CJob
{
public:
	CClientSocketJob()
	{
		init();
	}
	~CClientSocketJob()
	{
		clear();
	}

	void init();
	void clear();
	bool initSocket();
	virtual void doing(int uParam);
	/// 清除socket
	void clearSocket();
	void onReceivePlayerMsg();

public:
	CTcpSocket&				getTcpSocket(){return mTcpSocket;}
	/// 通过socket索引得到消息队列
	CPlayerMsgQueue*		getPlayerMsgQueue(int nSocketIndex);
	/// 通过socket索引得到接收消息队列
	CByteStream*			getReceiveMsgQueue(int nSocketIndex);
	/// 增加socket索引对应的消息队列
	CPlayerMsgQueue*		addPlayerMsgQueue(int nSocketIndex);

private:
	CTcpSocket				mTcpSocket;					// Socket
	byte*					mpSocketBuff;				// Socket Buff
	CSelectModel			mSelectModel;				// select 模型

#ifdef MYTH_OS_UNIX
	typedef std::vector<CPlayerMsgQueue*>				PLAYER_MSG_QUEUE_LIST;

#else
	typedef std::map<unsigned int, CPlayerMsgQueue*>	PLAYER_MSG_QUEUE_LIST;
#endif
	PLAYER_MSG_QUEUE_LIST	mPlayerSocketList;

};
#endif