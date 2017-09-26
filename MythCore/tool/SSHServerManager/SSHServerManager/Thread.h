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
	emThreadState_None		= 0,			// 未知状态
	emThreadState_UnInit	= 1,			// 尚未初始化
	emThreadState_Initing	= 2,			// 正在初始化
	emThreadState_Running	= 3,			// 正在运行中
	emThreadState_Stopping	= 4,			// 正在准备或者停止中
	emThreadState_Stopped	= 5,			// 已经停止
	emThreadStateMax
};

class CThread
{
public:
	CThread(void);
	virtual ~CThread(void);


public:
	// 运行前调用
	virtual void PrepareRun();
	// 运行线程
	virtual void Run(); 
	// 停止前调用
	virtual void PrepareStop();

public:
	int			CreateThread();
	void		StopThread(int nTime, void* pArg);
	void		Suspend();
	void		Resume();

public:
	// 是否是当前纯程
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