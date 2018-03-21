#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "threadpool.h"
#include "log.h"
#include "dbjob.h"
#include "locallogjob.h"
#include "singleton.h"
#include "scenejob.h"

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
	bool		init();
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
	/// 退出
	void		exit();

public:
	void		pushTask(EmTaskType eTaskType, CInternalMsg* pMsg);
	void		pushDBTask(int nUid, byte* pData, int nDataLength);

	short getServerID() const { return mServerID; }
	void setServerID(short nValue) { mServerID = nValue; }
private:
	CLog*					mDefaultLog;

	CDBJob					mDBJob[MAX_DB_JOB];
	CLocalLogJob			mLocalLogJob;
	CSceneJob				mSceneJob;

	// 线程池
	Myth::CThreadPool		mThreadPool;
	// 服务器对应的服务器ID，服务器的唯一ID
	short					mServerID;
};
#endif