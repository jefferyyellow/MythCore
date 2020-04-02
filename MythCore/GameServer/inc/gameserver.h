#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "log.h"
#include "singleton.h"
#include "timemanager.h"
using namespace Myth;

class CJobManager;
class CGameServer : public CSingleton<CGameServer>
{
	friend class CSingleton<CGameServer>;
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

private:
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
};
#endif