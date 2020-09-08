#ifndef __JOBMANAGER_H__
#define __JOBMANAGER_H__
#include "singleton.h"
#include "threadpool.h"
#define MAX_DB_JOB		8
using namespace Myth;

enum EmJobTaskType
{
	emJobTaskType_None			= 0,			// 未知
	emJobTaskType_DB			= 1,			// DB
	emJobTaskType_LocalLog		= 2,			// 本地日志
	emJobTaskType_Scene			= 3,			// 场景
	emJobTaskType_Plat			= 4,			// 平台
	emJobTaskType_Rank			= 5,			// 排行榜
};

class CDBJob;
class CLocalLogJob;
class CSceneJob;
class CPlatJob;
class CRankJob;
class CInternalMsg;
class CJobManager : public CSingleton<CJobManager>
{
	friend class CSingleton<CJobManager>;

private:
	CJobManager();
	~CJobManager(){}

public:
	bool	init();
	/// 初始化线程
	bool	initThread();
	/// 初始化最小化
	bool	initMin();
	/// 运行
	void	run();
	/// 清理线程
	void	clearThread();
	/// 检查除了log job是否都已经退出
	bool	checkOtherJobExit();
	/// 检查所有的 job是否退出
	bool	checkAllJobExit();
	void	pushTask(EmJobTaskType eTaskType, CInternalMsg* pMsg);
	void	pushDBTask(int nUid, byte* pData, int nDataLength);
private:
	CDBJob*					mpDBJob[MAX_DB_JOB];
	CLocalLogJob*			mpLocalLogJob;


	CSceneJob*				mpSceneJob;
	CPlatJob*				mpPlatJob;
	CRankJob*				mpRankJob;

	// 线程池
	Myth::CThreadPool		mThreadPool;
};
#endif