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
	/// ������Ϣ�б�
	CByteStream	mReceiveStream;
	/// ������Ϣ�б�
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
	/// ���socket
	void clearSocket();
	void onReceivePlayerMsg();

public:
	CTcpSocket&				getTcpSocket(){return mTcpSocket;}
	/// ͨ��socket�����õ���Ϣ����
	CPlayerMsgQueue*		getPlayerMsgQueue(int nSocketIndex);
	/// ͨ��socket�����õ�������Ϣ����
	CByteStream*			getReceiveMsgQueue(int nSocketIndex);
	/// ����socket������Ӧ����Ϣ����
	CPlayerMsgQueue*		addPlayerMsgQueue(int nSocketIndex);

private:
	CTcpSocket				mTcpSocket;					// Socket
	byte*					mpSocketBuff;				// Socket Buff
	CSelectModel			mSelectModel;				// select ģ��

#ifdef MYTH_OS_UNIX
	typedef std::vector<CPlayerMsgQueue*>				PLAYER_MSG_QUEUE_LIST;

#else
	typedef std::map<unsigned int, CPlayerMsgQueue*>	PLAYER_MSG_QUEUE_LIST;
#endif
	PLAYER_MSG_QUEUE_LIST	mPlayerSocketList;

};
#endif