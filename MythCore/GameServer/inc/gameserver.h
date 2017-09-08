#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "threadpool.h"
#include "log.h"
#include "dbjob.h"
#include "locallogjob.h"
#include "singleton.h"
#include "scenejob.h"

using namespace Myth;

class CGameServer : public CSingleton<CGameServer>
{
	friend class CSingleton<CGameServer>;
private:
	CGameServer(){}
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

private:
	CLog*					mDefaultLog;
	Myth::CThreadPool*		mThreadPool;

	CDBJob					mDBJob;
	CLocalLogJob			mLocalLogJob;
	CSceneJob				mSceneJob;
};
#endif