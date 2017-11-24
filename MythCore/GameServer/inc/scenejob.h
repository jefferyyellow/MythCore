#ifndef __SCENEJOB_H__
#define __SCENEJOB_H__
#include "job.h"
#include "sharememory.h"
#include "socketstream.h"
#include "messagefactory.h"
#include "singleton.h"
#include "logintype.h"
#include "dbmessage.h"
#include <map>
using namespace Myth;

#define PIPE_SIZE					((int)0x1000000)	/*内存管道的大小*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket缓冲区大小

class CEntityPlayer;
class CSceneJob : public CJob < 1000, 100 >, public CSingleton<CSceneJob>
{
	friend class CSingleton<CSceneJob>;
	/// 键是role id,值是obj id
	typedef std::map<uint32, uint32> PLAYER_LIST;
	/// 键是socket index, 值是obj id
	typedef std::map<uint32, uint32> PLAYER_SOCKET_LIST;

public:
	CSceneJob(){}
	~CSceneJob(){}

public:
	bool		init(int nDBBuffSize);

public:
	virtual void doing(uint32 uParam);

public:
	/// 发送前端消息
	void		sendClientMessage(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage);
	void		sendClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);

private:
	void		onTask(CInternalMsg* pMsg);
	/// 处理前端消息
	void		processClientMessage();
	/// 分发前端消息
	void		dispatchClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// 初始化共享内存
	bool		initShareMemory();
	/// 压入DB数据
	void		pushDBData(uint8* pData, int nDataLength);
	/// 取出DB数据
	void		popDBData(uint8* pData, int &rLength);
	/// 压入DB任务
	void		pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, char* pSql, ...);

private:
	CShareMemory*			mShareMemory;
	CSocketStream*			mTcp2ServerMemory;
	CSocketStream*			mServer2TcpMemory;
	char					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];

	PLAYER_LIST				mPlayerList; 
	PLAYER_SOCKET_LIST		mPlayerSocketList;


	CSocketStream			mDBStream;
	uint8*					mDBBuffer;
	CSimpleLock				mDBStreamLock;

	CDBRequest				mDBRequest;
	CDBResponse				mDBResponse;
};
#endif