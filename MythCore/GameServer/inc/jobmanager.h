#ifndef __JOBMANAGER_H__
#define __JOBMANAGER_H__
#include "singleton.h"
#include "threadpool.h"
#define MAX_DB_JOB		8
#define MAX_MAP_JOB		20
using namespace Myth;

enum EmJobType
{
	emJobTaskType_None			= 0,			// δ֪
	emJobTaskType_DB			= 1,			// DB
	emJobTaskType_LocalLog		= 2,			// ������־
	emJobTaskType_Scene			= 3,			// ����
	emJobTaskType_Plat			= 4,			// ƽ̨
	emJobTaskType_Rank			= 5,			// ���а�
	emJobTaskType_Login			= 6,			// ��½
	emJobTaskType_ClientSocket	= 7,			// ǰ��socket
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
	/// ��ʼ���߳�
	bool	initThread();
	/// ��ʼ����С��
	bool	initMin();
	/// ����
	void	run();
	/// �����߳�
	void	clearThread();
	/// ������log job�Ƿ��Ѿ��˳�
	bool	checkOtherJobExit();
	/// ������е� job�Ƿ��˳�
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
	
	// �̳߳�
	Myth::CThreadPool		mThreadPool;
};
#endif