#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "threadpool.h"
#include "log.h"
#include "sharememory.h"
#include "socketstream.h"
#include "messagefactory.h"
#include "dbjob.h"
#define PIPE_SIZE					((int)0x1000000)	/*�ڴ�ܵ��Ĵ�С*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket��������С

using namespace Myth;

struct CExchangeHead
{
	uint32	mTcpIndex;
	uint8	mTcpState;
};

class CGameServer
{
public:
	CGameServer(){}
	~CGameServer(){}

public:
	/// ��ʼ��
	bool		init();
	/// ��ʼ��־
	bool		initLog();
	/// ��ʼ�������ڴ�
	bool		initShareMemory();
	/// ��ʼ�߼�ģ��
	bool		initLogicModule();
	/// ��ʼ�߳�
	bool		initThread();

	/// ����
	void		run();
	/// ����ǰ����Ϣ
	void		processClientMessage();
	/// �ַ�ǰ����Ϣ
	void		dispatchClientMessage(unsigned short nMessageID, Message* pMessage);

	/// ��ʼΪ�˳���׼��
	void		clear();
	/// ������־��Դ
	void		clearLog();
	/// �˳�
	void		exit();

private:
	CLog*					mDefaultLog;
	Myth::CThreadPool*		mThreadPool;

	CShareMemory*			mShareMemory;
	CSocketStream*			mTcp2ServerMemory;
	CSocketStream*			mServer2TcpMemory;
	char					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];

	CDBJob					mDBJob;

};
#endif