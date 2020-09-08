#ifndef __JOBMANAGER_H__
#define __JOBMANAGER_H__
#include "singleton.h"
#include "threadpool.h"
#define MAX_DB_JOB		8
using namespace Myth;

enum EmJobTaskType
{
	emJobTaskType_None			= 0,			// δ֪
	emJobTaskType_DB			= 1,			// DB
	emJobTaskType_LocalLog		= 2,			// ������־
	emJobTaskType_Scene			= 3,			// ����
	emJobTaskType_Plat			= 4,			// ƽ̨
	emJobTaskType_Rank			= 5,			// ���а�
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
	void	pushTask(EmJobTaskType eTaskType, CInternalMsg* pMsg);
	void	pushDBTask(int nUid, byte* pData, int nDataLength);
private:
	CDBJob*					mpDBJob[MAX_DB_JOB];
	CLocalLogJob*			mpLocalLogJob;


	CSceneJob*				mpSceneJob;
	CPlatJob*				mpPlatJob;
	CRankJob*				mpRankJob;

	// �̳߳�
	Myth::CThreadPool		mThreadPool;
};
#endif