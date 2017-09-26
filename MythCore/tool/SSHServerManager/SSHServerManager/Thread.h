#ifndef __THREAD_H__
#define __THREAD_H__
#include "Commondefine.h"

// typedef void* (*THREAD_FUNCTION)(void* arg);
#ifdef LINUX
void*
#else
DWORD WINAPI
#endif
ThreadFunction(void* arg);

enum TmThreadState
{
	emThreadState_None		= 0,			// δ֪״̬
	emThreadState_UnInit	= 1,			// ��δ��ʼ��
	emThreadState_Initing	= 2,			// ���ڳ�ʼ��
	emThreadState_Running	= 3,			// ����������
	emThreadState_Stopping	= 4,			// ����׼������ֹͣ��
	emThreadState_Stopped	= 5,			// �Ѿ�ֹͣ
	emThreadStateMax
};

class CThread
{
public:
	CThread(void);
	virtual ~CThread(void);


public:
	// ����ǰ����
	virtual void PrepareRun();
	// �����߳�
	virtual void Run(); 
	// ֹͣǰ����
	virtual void PrepareStop();

public:
	int			CreateThread();
	void		StopThread(int nTime, void* pArg);
	void		Suspend();
	void		Resume();

public:
	// �Ƿ��ǵ�ǰ����
	bool		IsSelfTread(pthread_t tTreadID)
	{
#ifdef LINUX
		return pthread_equal(mThreadID, tTreadID);
#else
		return mThreadID == tTreadID;
#endif
	}

	
public:
	pthread_t	GetThreadID(){return mThreadID;}
	void		SetThreadID(pthread_t tThreadID){mThreadID = tThreadID;}

	TmThreadState	GetThreadState(){return mThreadState;}
	void			SetThreadState(TmThreadState eState){mThreadState = eState;}

public:
	pthread_t		mThreadID;
	TmThreadState	mThreadState;
#ifdef WIN32
	HANDLE			mThreadHand;
#endif

#ifdef LINUX
	pthread_mutex_t mMutex;
	pthread_cond_t	mCond;
#endif

};
#endif