#ifndef __SCENEJOB_H__
#define __SCENEJOB_H__
#include "job.h"
#include "sharememory.h"
#include "socketstream.h"
#include "messagefactory.h"
#include "singleton.h"
#include "logintype.h"
#include "bytestream.h"
#include "timemanager.h"
#include <map>
#include <list>

using namespace Myth;

#define PIPE_SIZE					((int)0x1000000)	/*内存管道的大小*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket缓冲区大小
#define CHECK_NEW_DAY_INTERVAL		30000				// 30秒检查一次新的一天
enum EmServerState
{
	emServerStateInit		= 0,	// 初始状态
	emServerStateLaunch		= 1,	// 启动状态
	emServerStateRun		= 2,	// 游戏状态
	emServerStateExit		= 3,	// 退出状态
};

class CEntityPlayer;
class CLogicModule;
class CEntity;
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
	CSceneJob();
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
	// 状态少，暂时不用状态机了
	void		doInit();
	void		doLaunch();
	void		doRun();
	void		doExit();

public:
	void		launchServer();
	bool		checkLaunch();
	void		launchComplete();
	void		exitServer();
	void		newDayCome();
	void		newWeekCome();
	void		createPlayer(CEntityPlayer* pPlayer);
	void		destroyPlayer(CEntityPlayer* pPlayer);
	void		timer(unsigned int nTickOffset);

public:
	void		checkNewDayCome();

public:
	/// 发送前端消息
	void		send2Player(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage);
	void		send2Player(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// 发生给所有的玩家消息
	void		send2AllPlayer(unsigned short nMessageID, Message* pMessage);
	/// 断开玩家的连接
	void		disconnectPlayer(CExchangeHead& rExchangeHead);
	void		disconnectPlayer(CEntityPlayer* pPlayer);
	/// 登录了一个玩家（只是登录校验完成，数据还没有加载完成）
	bool		onPlayerLogin(CEntityPlayer* pNewPlayer);
	/// 离开了一个玩家
	void		onPlayerLeaveGame(CEntityPlayer* pPlayer);
	/// 一个Socket断开
	void		onSocketDisconnect(int nSocketIndex);
	/// 得到所有的玩家列表
	PLAYER_LIST& getPlayerList(){	return mPlayerList;	}
	/// 得到所有玩家的socket列表
	PLAYER_SOCKET_LIST& getPlayerSocketList(){return mPlayerSocketList;};
	/// 通过角色ID得到玩家
	CEntityPlayer* getPlayerByRoleID(unsigned int nRoleID);
	/// 通过SocketIndex得到玩家
	CEntityPlayer* getPlayerBySocketIndex(short nSocketIndex);
	/// 添加socket index
	bool			addPlayerSocketIndex(short nSocketIndex, int nObjID);
	/// 删除socket index
	void			removePlayerSocketIndex(short nSocketIndex);
private:
	void		onTask(CInternalMsg* pMsg);
	/// 处理前端消息
	void		processClientMessage();
	/// 分发前端消息
	void		dispatchClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// 初始化共享内存
	bool		initShareMemory();

public:
	/// 服务器状态
	EmServerState getServerState() const { return mServerState; }
	void setServerState(EmServerState nValue) { mServerState = nValue; }

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
	/// 服务器状态
	EmServerState			mServerState;
	/// 新的一天检查计时器
	CAutoResetTimer			mNewDayTimer;
};
#endif