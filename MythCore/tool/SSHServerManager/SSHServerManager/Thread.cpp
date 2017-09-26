#include "stdafx.h"
#include "Thread.h"

#ifdef LINUX
void*
#else
DWORD WINAPI
#endif
ThreadFunction(void* arg)
{
	CThread* pThis = (CThread*)arg;
	if (NULL == pThis)
	{
#ifdef LINUX
		return NULL;
#else
		return -1;
#endif
	}
	else
	{
		pThis->PrepareRun();
		pThis->SetThreadState(emThreadState_Running);
		pThis->Run();
		pThis->SetThreadState(emThreadState_Stopped);
	}


#ifdef LINUX
	return NULL;
#else
	return 0;
#endif
}

CThread::CThread(void)
{
	mThreadID = 0;
	mThreadState = emThreadState_UnInit;
}

CThread::~CThread(void)
{
	
}

int CThread::CreateThread()
{
#ifdef LINUX
	pthread_cond_init(&mCond, NULL);
	pthread_mutex_init(&mMutex, NULL);
	return pthread_create( &mThreadID, NULL, ThreadFunction, (void *)this );
#else
	mThreadHand = ::CreateThread(NULL, 0, ThreadFunction, (void*)this, 0, (LPDWORD)&mThreadID);
	if(NULL == mThreadHand)
	{
		return GetLastError();
	}
	return 0;
#endif
}

void CThread::StopThread(int nTime, void* pArg)
{
	SetThreadState(emThreadState_Stopping);
#ifdef LINUX
	pthread_join(mThreadID, NULL);
	pthread_exit(pArg);
#else
	WaitForSingleObject(mThreadHand, nTime);
	CloseHandle(mThreadHand);
#endif
}

void CThread::Suspend()
{
#ifdef LINUX
	pthread_mutex_lock(&mMutex);
	pthread_cond_wait(&mCond, &mMutex);
	pthread_mutex_unlock(&mMutex);
#else
#endif
}

void CThread::Resume()
{
#ifdef LINUX
	pthread_mutex_lock(&mMutex);
	pthread_cond_signal(&mCond);
	pthread_mutex_unlock(&mMutex);
#else
#endif
}


void CThread::PrepareRun()
{
	SetThreadState(emThreadState_Initing);
}
// Í£Ö¹Ç°µ÷ÓÃ
void CThread::PrepareStop()
{
	SetThreadState(emThreadState_Stopping);
}

void CThread::Run()
{
	int i = 0;
	while (mThreadState == emThreadState_Running)
	{
		printf("%d\n", i);
#ifdef LINUX
		sleep(2);
#else
		Sleep(2000);
#endif
		++ i;
	}
}