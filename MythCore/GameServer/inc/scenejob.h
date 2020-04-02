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
class CLoginPlayer;
class CEntityTimer;
class CSceneJob : public CJob < 100, 100 >, public CSingleton<CSceneJob>
{
public:
	friend class CSingleton<CSceneJob>;
	/// 键是role id,值是obj id
	typedef std::map<unsigned int, int> PLAYER_LIST;
	/// 键是socket index, 值是obj id
	typedef std::map<unsigned int, int> PLAYER_SOCKET_LIST;
	/// 逻辑模块列表
	typedef std::list<CLogicModule*> LOGIC_MODULE_LIST;
private:
	CSceneJob();
	~CSceneJob(){}

public:
	void		init()
	{
        mShareMemory = NULL;
        mTcp2ServerMemory = NULL;
        mServer2TcpMemory = NULL;
        mDBBuffer = NULL;
        mLastTimerTick = 0;
        mLastTime = 0;
        mServerState = emServerStateInit;
        mMorningTime = 0;
	}
	bool		initBase(int nDBBuffSize);
	void		clearBase();

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
	void		onNewDayCome();

public:
	void		send2Player(CLoginPlayer* pLoginPlayer, unsigned short nMessageID, Message* pMessage);
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
	/// 通过角色ID得到玩家
	CEntityPlayer* getPlayerByRoleID(unsigned int nRoleID);
	/// 通过SocketIndex得到玩家
	CEntityPlayer* getPlayerBySocketIndex(short nSocketIndex);
	/// 添加socket index
	bool			addPlayerSocketIndex(short nSocketIndex, int nObjID);
	/// 删除socket index
	void			removePlayerSocketIndex(short nSocketIndex);
	/// 记录perf日志
	void			logPerf();
private:
	/// 发送前端消息
	void		send2Player(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage);
	void		onTask(CInternalMsg* pMsg);
	/// 处理前端消息
	void		processClientMessage();
	/// 分发前端消息
	void		dispatchClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// 初始化共享内存
	bool		initShareMemory();

public:
	/// 计时器到时回调函数
	static void		timeOutCallFunc(CEntityTimer* pTimer);
public:
	tm&				getTmNow(){ return mTmNow; }
	void			setTmNow(time_t tTimeNow)
	{
#ifdef MYTH_OS_WINDOWS
		localtime_s(&mTmNow, &tTimeNow);
#else
		localtime_r(&tTimeNow, &mTmNow);
#endif // MYTH_OS_WINDOWS
	}
public:
	/// autocode don't edit
    CShareMemory* getShareMemory(){ return mShareMemory;}
    void setShareMemory(CShareMemory* value){ mShareMemory = value;}

    CSocketStream* getTcp2ServerMemory(){ return mTcp2ServerMemory;}
    void setTcp2ServerMemory(CSocketStream* value){ mTcp2ServerMemory = value;}

    CSocketStream* getServer2TcpMemory(){ return mServer2TcpMemory;}
    void setServer2TcpMemory(CSocketStream* value){ mServer2TcpMemory = value;}

    PLAYER_LIST& getPlayerList(){ return mPlayerList;}

    PLAYER_SOCKET_LIST& getPlayerSocketList(){ return mPlayerSocketList;}

    CByteStream& getDBStream(){ return mDBStream;}

    byte* getDBBuffer(){ return mDBBuffer;}
    void setDBBuffer(byte* value){ mDBBuffer = value;}

    CSimpleLock& getDBStreamLock(){ return mDBStreamLock;}

    LOGIC_MODULE_LIST& getLogicModuleList(){ return mLogicModuleList;}

    uint64 getLastTimerTick(){ return mLastTimerTick;}
    void setLastTimerTick(uint64 value){ mLastTimerTick = value;}

    time_t getLastTime(){ return mLastTime;}
    void setLastTime(time_t value){ mLastTime = value;}

    EmServerState getServerState(){ return mServerState;}
    void setServerState(EmServerState value){ mServerState = value;}

    time_t getMorningTime(){ return mMorningTime;}
    void setMorningTime(time_t value){ mMorningTime = value;}

    CAutoResetTimer& getMinuteTimer(){ return mMinuteTimer;}
	/// end autocode
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
	/// 上次刷新的时间
	time_t					mLastTime;
	/// 服务器状态,default:emServerStateInit
	EmServerState			mServerState;
	/// 当前的时间
	struct tm				mTmNow;
	/// 指定时间的那天早上（00：00：00）
	time_t					mMorningTime;
	/// 秒计时器,按秒计数
	CAutoResetTimer			mMinuteTimer;
};
#endif