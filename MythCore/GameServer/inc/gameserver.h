#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "log.h"
#include "singleton.h"
#include "timemanager.h"
using namespace Myth;

class CJobManager;
class CGameServer : public CSingleton<CGameServer>
{
	friend class CSingleton<CGameServer>;
private:
	CGameServer();
	~CGameServer(){}

public:
	/// ��ʼ��
	void		init();
	/// ��ʼ��
	bool		initAll();
	/// ��ʼ��־
	bool		initLog();
	/// ��ʼ�߼�ģ��
	bool		initLogicModule();
	/// ��ʼ����Ϸ��̬����
	bool		initStaticData();

	/// ����
	void		run();

	/// ��ʼΪ�˳���׼��
	void		clear();
	/// ������־��Դ
	void		clearLog();
	/// �����߼�ģ��
	void		clearLogicModule();
	/// ����̬����
	void		clearStaticData();
	/// �˳�
	void		exit();
	/// ��¼���е�Perf�ļ�¼
	void		logPerf();

public:
	short		getServerID() const { return mServerID; }
	void		setServerID(short nValue) { mServerID = nValue; }

	bool		getExit() const { return mExit; }
	void		setExit(bool nValue) { mExit = nValue; }

private:
#ifdef __DEBUG__
	CLog*					mDefaultLog;
#endif
	CJobManager*			mpJobManager;
	// ��������Ӧ�ķ�����ID����������ΨһID
	short					mServerID;
	// �˳�״̬
	bool					mExit;
	/// ���ʱ��,�������
	CAutoResetTimer			mMinuteTimer;
	/// �ϴ�ˢ�µ�ʱ��
	time_t					mLastTime;
};
#endif