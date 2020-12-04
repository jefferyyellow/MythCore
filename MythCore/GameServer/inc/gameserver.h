#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "log.h"
#include "singleton.h"
#include "timeutility.h"
#include "gameservernet.h"
using namespace Myth;

class CJobManager;

enum EmServerState
{
	emServerStateInit		= 0,	// 初始状态
	emServerStateLaunch		= 1,	// 启动状态
	emServerStateRun		= 2,	// 游戏状态
	emServerStateExit		= 3,	// 退出状态
};

class CGameServer : public CSingleton<CGameServer>
{
	friend class CSingleton<CGameServer>;
public:
	/// 键是role id,值是obj id
	typedef std::map<unsigned int, int> PLAYER_LIST;
	/// 键是socket index, 值是obj id
	typedef std::map<unsigned int, int> PLAYER_SOCKET_LIST;

private:
	CGameServer();
	~CGameServer(){}

public:
	/// 初始化
	void		init();
	/// 初始化
	bool		initAll();
	/// 初始日志
	bool		initLog();
	/// 初始逻辑模块
	bool		initLogicModule();
	/// 初始化游戏静态数据
	bool		initStaticData();
	/// 初始化服务器网络
	bool		initNet();

	/// 运行
	void		run();

	/// 开始为退出做准备
	void		clear();
	/// 清理日志资源
	void		clearLog();
	/// 清理逻辑模块
	void		clearLogicModule();
	/// 清理静态数据
	void		clearStaticData();
	/// 退出
	void		exit();
	/// 记录所有的Perf的记录
	void		logPerf();

public:
	short		getServerID() const { return mServerID; }
	void		setServerID(short nValue) { mServerID = nValue; }

	bool		getExit() const { return mExit; }
	void		setExit(bool nValue) { mExit = nValue; }

	time_t		GetGameTimeOffset() const { return mGameTimeOffset; }
	void		SetGameTimeOffset(time_t nValue) { mGameTimeOffset = nValue; }



	///// 断开玩家的连接
	//void		disconnectPlayer(CExchangeHead& rExchangeHead);
	//void		disconnectPlayer(CEntityPlayer& rPlayer);
	///// 登录了一个玩家（只是登录校验完成，数据还没有加载完成）
	//bool		onPlayerLogin(CEntityPlayer* pNewPlayer);
	///// 离开了一个玩家
	//void		destroyPlayerObject(CEntityPlayer& rPlayer);
	///// 一个Socket断开
	//void		onSocketDisconnect(int nSocketIndex);
	///// 通过角色ID得到玩家
	//CEntityPlayer* getPlayerByRoleID(unsigned int nRoleID);
	///// 通过SocketIndex得到玩家
	//CEntityPlayer* getPlayerBySocketIndex(short nSocketIndex);
	///// 添加socket index
	//bool			addPlayerSocketIndex(short nSocketIndex, int nObjID);
	///// 删除socket index
	//void			removePlayerSocketIndex(short nSocketIndex);

private:
	CGameServerNet			mGameServerNet;
#ifdef __DEBUG__
	CLog*					mDefaultLog;
#endif
	CJobManager*			mpJobManager;
	// 服务器对应的服务器ID，服务器的唯一ID
	short					mServerID;
	// 退出状态
	bool					mExit;
	/// 秒计时器,按秒计数
	CAutoResetTimer			mMinuteTimer;
	/// 上次刷新的时间
	time_t					mLastTime;
	/// 游戏服务器偏移时间
	time_t					mGameTimeOffset;

	PLAYER_LIST				mPlayerList;
	PLAYER_SOCKET_LIST		mPlayerSocketList;

};
#endif