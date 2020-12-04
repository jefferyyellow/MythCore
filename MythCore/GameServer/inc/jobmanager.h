#ifndef __JOBMANAGER_H__
#define __JOBMANAGER_H__
#include "singleton.h"
#include "threadpool.h"
#define MAX_DB_JOB		8
#define MAX_MAP_JOB		20
using namespace Myth;

enum EmJobType
{
	emJobTaskType_None			= 0,			// 未知
	emJobTaskType_DB			= 1,			// DB
	emJobTaskType_LocalLog		= 2,			// 本地日志
	emJobTaskType_Scene			= 3,			// 场景
	emJobTaskType_Plat			= 4,			// 平台
	emJobTaskType_Rank			= 5,			// 排行榜
	emJobTaskType_Login			= 6,			// 登陆
	emJobTaskType_ClientSocket	= 7,			// 前端socket
};

class CDBJob;
class CLocalLogJob;
class CMapJob;
class CPlatJob;
class CRankJob;
class CLoginJob;
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
	void	pushTaskByType(EmJobType eJobType, CInternalMsg* pMsg);
	void	pushTaskByID(byte nJobID, CInternalMsg* pMsg);
	void	pushDBTask(int nUid, CInternalMsg* pMsg);
	void	pushBackJob(IJob* pJob);
private:
	CDBJob*					mpDBJob[MAX_DB_JOB];
	CMapJob*				mpMapJob[MAX_MAP_JOB];
	CLocalLogJob*			mpLocalLogJob;
	CLoginJob*				mpLoginJob;

	CPlatJob*				mpPlatJob;
	CRankJob*				mpRankJob;
	
	// 线程池
	Myth::CThreadPool		mThreadPool;
};
#endif