#ifndef __SERVERRESAVE_H__
#define __SERVERRESAVE_H__
#include "commontype.h"
#include "bytestream.h"
#include "threadpool.h"
#include "dbjob.h"
#include "locallogjob.h"
#include "singleton.h"
#include "log.h"
#include "timeutility.h"
#define MAX_DB_JOB		8

using namespace Myth;
class CJobManager;
class CServerResave : public CSingleton <CServerResave>
{
	friend class CSingleton < CServerResave > ;
private:
	CServerResave();
	~CServerResave(){}


public:
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
	/// 重置玩家存盘状态
	void		ResetPlayerSaveStatus();
	/// 比例保存玩家
	void		SavePlayerBat();
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


public:
	/// 压入DB数据
	void		pushBackDBData(byte* pData, int nDataLength);
	/// 取出DB数据
	void		popUpDBData(byte* pData, int &rLength);

public:
	bool		getExit() const { return mExit; }
	void		setExit(bool nValue) { mExit = nValue; }

private:
		/// 
		CJobManager*			mpJobManager;
		/// 上次刷新的时间
		time_t					mLastTime;
		// 退出状态
		bool					mExit;
		///	5秒计时器
		CAutoResetTimer			mFiveSecTimer;
};
#endif
