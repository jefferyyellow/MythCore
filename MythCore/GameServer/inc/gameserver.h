#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "threadpool.h"
#include "log.h"
#include "dbjob.h"
#include "locallogjob.h"
#include "singleton.h"
#include "scenejob.h"
#include "platjob.h"
#define MAX_DB_JOB		8
using namespace Myth;

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
	/// 初始线程
	bool		initThread();

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
	/// 清理线程
	void		clearThread();
	/// 退出
	void		exit();
	/// 检查除了log job是否都已经退出
	bool		checkOtherJobExit();
	/// 检查所有的 job是否退出
	bool		checkAllJobExit();

public:
	void		pushTask(EmJobTaskType eTaskType, CInternalMsg* pMsg);
	void		pushDBTask(int nUid, byte* pData, int nDataLength);

	short		getServerID() const { return mServerID; }
	void		setServerID(short nValue) { mServerID = nValue; }

	bool		getExit() const { return mExit; }
	void		setExit(bool nValue) { mExit = nValue; }

private:
	CLog*					mDefaultLog;

	CDBJob					mDBJob[MAX_DB_JOB];
	CLocalLogJob			mLocalLogJob;
	CSceneJob				mSceneJob;
	CPlatJob				mPlatJob;

	// 线程池
	Myth::CThreadPool		mThreadPool;
	// 服务器对应的服务器ID，服务器的唯一ID
	short					mServerID;
	// 退出状态
	bool					mExit;
};
#endif