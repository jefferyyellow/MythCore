#ifndef __SERVERRESAVE_H__
#define __SERVERRESAVE_H__
class CServerResave
{


private:
#ifdef __DEBUG__
		CLog*					mDefaultLog;
#endif
		// ���ݿ⴦��
		CDBJob					mDBJob[MAX_DB_JOB];
		// �̳߳�
		Myth::CThreadPool		mThreadPool;
};
#endif
