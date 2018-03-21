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
	/// ��ʼ��
	bool		init();
	/// ��ʼ��־
	bool		initLog();
	/// ��ʼ�߼�ģ��
	bool		initLogicModule();
	/// ��ʼ����Ϸ��̬����
	bool		initStaticData();
	/// ��ʼ�߳�
	bool		initThread();

	/// ����
	void		run();

	/// ��ʼΪ�˳���׼��
	void		clear();
	/// ������־��Դ
	void		clearLog();
	/// �˳�
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

	// �̳߳�
	Myth::CThreadPool		mThreadPool;
	// ��������Ӧ�ķ�����ID����������ΨһID
	short					mServerID;
};
#endif