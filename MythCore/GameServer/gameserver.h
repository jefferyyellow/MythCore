#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "threadpool.h"
#include "log.h"
#include "sharememory.h"
#include "socketstream.h"
#include "messagefactory.h"
#include "dbjob.h"
#include "locallogjob.h"
#include "singleton.h"
#define PIPE_SIZE					((int)0x1000000)	/*内存管道的大小*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket缓冲区大小

using namespace Myth;

struct CExchangeHead
{
	uint32	mTcpIndex;
	uint8	mTcpState;
};

class CGameServer : public CSingleton<CGameServer>
{
	friend class CSingleton<CGameServer>;
private:
	CGameServer(){}
	~CGameServer(){}

public:
	/// 初始化
	bool		init();
	/// 初始日志
	bool		initLog();
	/// 初始化共享内存
	bool		initShareMemory();
	/// 初始逻辑模块
	bool		initLogicModule();
	/// 初始线程
	bool		initThread();

	/// 运行
	void		run();
	/// 处理前端消息
	void		processClientMessage();
	/// 分发前端消息
	void		dispatchClientMessage(unsigned short nMessageID, Message* pMessage);

	/// 开始为退出做准备
	void		clear();
	/// 清理日志资源
	void		clearLog();
	/// 退出
	void		exit();

public:
	void		pushTask(EmTaskType eTaskType, CInternalMsg* pMsg);

private:
	CLog*					mDefaultLog;
	Myth::CThreadPool*		mThreadPool;

	CShareMemory*			mShareMemory;
	CSocketStream*			mTcp2ServerMemory;
	CSocketStream*			mServer2TcpMemory;
	char					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];

	CDBJob					mDBJob;
	CLocalLogJob			mLocalLogJob;
};
#endif