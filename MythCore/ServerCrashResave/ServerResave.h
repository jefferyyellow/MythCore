#ifndef __SERVERRESAVE_H__
#define __SERVERRESAVE_H__
#include "commontype.h"
#include "bytestream.h"
#include "threadpool.h"
#include "dbjob.h"
#include "locallogjob.h"
#include "singleton.h"
#include "log.h"
#include "timeutility.h"
#define MAX_DB_JOB		8

using namespace Myth;
class CJobManager;
class CServerResave : public CSingleton <CServerResave>
{
	friend class CSingleton < CServerResave > ;
private:
	CServerResave();
	~CServerResave(){}


public:
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
	/// ������Ҵ���״̬
	void		ResetPlayerSaveStatus();
	/// �����������
	void		SavePlayerBat();
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


public:
	/// ѹ��DB����
	void		pushBackDBData(byte* pData, int nDataLength);
	/// ȡ��DB����
	void		popUpDBData(byte* pData, int &rLength);

public:
	bool		getExit() const { return mExit; }
	void		setExit(bool nValue) { mExit = nValue; }

private:
		/// 
		CJobManager*			mpJobManager;
		/// �ϴ�ˢ�µ�ʱ��
		time_t					mLastTime;
		// �˳�״̬
		bool					mExit;
		///	5���ʱ��
		CAutoResetTimer			mFiveSecTimer;
};
#endif
