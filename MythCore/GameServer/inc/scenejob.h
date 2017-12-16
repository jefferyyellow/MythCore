#ifndef __SCENEJOB_H__
#define __SCENEJOB_H__
#include "job.h"
#include "sharememory.h"
#include "socketstream.h"
#include "messagefactory.h"
#include "singleton.h"
#include "logintype.h"
#include "bytestream.h"
#include <map>
#include <list>
using namespace Myth;

#define PIPE_SIZE					((int)0x1000000)	/*内存管道的大小*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket缓冲区大小

class CEntityPlayer;
class CLogicModule;
class CSceneJob : public CJob < 1000, 100 >, public CSingleton<CSceneJob>
{
public:
	friend class CSingleton<CSceneJob>;
	/// 键是role id,值是obj id
	typedef std::map<unsigned int, int> PLAYER_LIST;
	/// 键是socket index, 值是obj id
	typedef std::map<unsigned int, int> PLAYER_SOCKET_LIST;
	/// 逻辑模块列表
	typedef std::list<CLogicModule*> LOGIC_MODULE_LIST;
public:
	CSceneJob(){}
	~CSceneJob(){}

public:
	bool		init(int nDBBuffSize);

public:
	/// 压入DB数据
	void		pushBackDBData(byte* pData, int nDataLength);
	/// 取出DB数据
	void		popUpDBData(byte* pData, int &rLength);
	/// 检查DB流
	void		checkDBStream();

public:
	virtual void doing(int uParam);

public:
	/// 发送前端消息
	void		send2Player(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage);
	void		send2Player(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// 断开玩家的连接
	void		disconnectPlayer(CExchangeHead& rExchangeHead);
	void		disconnectPlayer(CEntityPlayer* pPlayer);
	/// 登录了一个玩家（只是登录校验完成，数据还没有加载完成）
	bool		onPlayerLogin(CEntityPlayer* pNewPlayer);
	/// 离开了一个玩家
	void		onPlayerLeaveGame(CEntityPlayer* pPlayer);
	/// 时间函数
	void		OnTimer(unsigned int nTickOffset);
	/// 得到所有的玩家列表
	PLAYER_LIST& getPlayerList(){	return mPlayerList;	}
	/// 得到所有玩家的socket列表
	PLAYER_SOCKET_LIST& getPlayerSocketList(){return mPlayerSocketList;};
	/// 通过角色ID得到玩家
	CEntityPlayer* getPlayerByRoleID(unsigned int nRoleID);
	/// 通过SocketIndex得到玩家
	CEntityPlayer* getPlayerBySocketIndex(short nSocketIndex);
private:
	void		onTask(CInternalMsg* pMsg);
	/// 处理前端消息
	void		processClientMessage();
	/// 分发前端消息
	void		dispatchClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// 初始化共享内存
	bool		initShareMemory();

private:
	CShareMemory*			mShareMemory;
	CSocketStream*			mTcp2ServerMemory;
	CSocketStream*			mServer2TcpMemory;
	char					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];

	PLAYER_LIST				mPlayerList; 
	PLAYER_SOCKET_LIST		mPlayerSocketList;


	CByteStream				mDBStream;
	byte*					mDBBuffer;
	CSimpleLock				mDBStreamLock;
	LOGIC_MODULE_LIST		mLogicModuleList;
	/// 上一次刷新计时器的时间
	uint64					mLastTimerTick;
};
#endif