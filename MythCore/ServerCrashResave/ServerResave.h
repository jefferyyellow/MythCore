#ifndef __SERVERRESAVE_H__
#define __SERVERRESAVE_H__
class CServerResave
{


private:
#ifdef __DEBUG__
		CLog*					mDefaultLog;
#endif
		// 数据库处理
		CDBJob					mDBJob[MAX_DB_JOB];
		// 线程池
		Myth::CThreadPool		mThreadPool;
};
#endif
