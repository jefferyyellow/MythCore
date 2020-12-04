#include "workthread.h"
#include "threadpool.h"
namespace Myth
{
#ifdef MYTH_OS_WINDOWS
	DWORD __stdcall ThreadFunction(void* arg)

#else 
	void* ThreadFunction(void* arg);
#endif
	{
		CThread* pThead = (CThread*)arg;
		if (NULL == pThead)
		{
			return 0;
		}

		pThead->setThreadState(emThreadState_Runing);
		while (true)
		{
			if (NULL == pThead->mpJob)
			{
				pThead->suspend();
				if (NULL != pThead->mpThreadPool)
				{
					IJob* pJob = pThead->mpThreadPool->popJob();
					if (NULL != pJob)
					{
						pThead->mpJob = pJob;
					}
				}
				continue;
			}
			pThead->mpJob->doing(pThead->getSerialNum());

			if (NULL != pThead->mpThreadPool)
			{
				pThead->mpThreadPool->setJobFree(pThead->mpJob);
				IJob* pJob = pThead->mpThreadPool->popJob();
				pThead->mpJob = pJob;
			}
		}

		return 0;
	}

	CThread::CThread()
	{
		mpThreadPool = NULL;
		mpJob = NULL;
		mThreadID = 0;
#ifdef MYTH_OS_WINDOWS
		mThreadHand = INVALID_HANDLE_VALUE;
#else
		pthread_cond_init(&mCond, NULL);
		pthread_mutexattr_init(&mMutexAttr);
		pthread_mutexattr_settype(&mMutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&mMutex, &mMutexAttr);
#endif
	}

	CThread::~CThread()
	{
#ifdef MYTH_OS_WINDOWS
		if (mThreadHand != INVALID_HANDLE_VALUE)
		{
			TerminateThread(mThreadHand, 0);
			CloseHandle(mThreadHand);
		}
		mThreadHand = INVALID_HANDLE_VALUE;
#else
		pthread_cond_destroy(&mCond);
		pthread_mutexattr_destroy(&mMutexAttr);
#endif
	}

	/// 开始（创建）线程
	void CThread::start()
	{
		if (emThreadState_Ready != getThreadState())
		{
			return;
		}
#ifdef MYTH_OS_WINDOWS
		mThreadHand = ::CreateThread(NULL, 0, ThreadFunction, (void*)this, 0, (LPDWORD)&mThreadID);
#else
		pthread_create(&mThreadID, NULL, ThreadFunction, (void *)this);
#endif
	}

	/// 终止线程
	void CThread::terminate()
	{
#ifdef MYTH_OS_WINDOWS
		if (mThreadHand == INVALID_HANDLE_VALUE)
		{
			return;
		}
		TerminateThread(mThreadHand, 0);
		CloseHandle(mThreadHand);
		mThreadHand = INVALID_HANDLE_VALUE;
#else
		if (0 == mThreadID)
		{
			return;
		}
		pthread_cancel(mThreadID);
#endif
	}

	/// 等待退出
	void CThread::wait()
	{
#ifdef MYTH_OS_WINDOWS
		if (mThreadHand == INVALID_HANDLE_VALUE)
		{
			return;
		}

		WaitForSingleObject(mThreadHand, INFINITE);
		CloseHandle(mThreadHand);
		mThreadHand = INVALID_HANDLE_VALUE;
#else

		if (0 == mThreadID)
		{
			return;
		}
		pthread_join(mThreadID, 0);
#endif
	}

	/// 暂停线程
	void CThread::suspend()
	{
#ifdef MYTH_OS_WINDOWS
		if (mThreadHand == INVALID_HANDLE_VALUE)
		{
			return;
		}
		::SuspendThread(mThreadHand);
#else
		if (0 == mThreadID)
		{
			return;
		}
		pthread_mutex_lock(&mMutex);
		pthread_cond_wait(&mCond, &mMutex);
		pthread_mutex_unlock(&mMutex);
#endif
	}

	/// 唤醒线程
	void CThread::resume()
	{
#ifdef MYTH_OS_WINDOWS
		if (mThreadHand == INVALID_HANDLE_VALUE)
		{
			return;
		}
		::ResumeThread(mThreadHand);
#else
		pthread_mutex_lock(&mMutex);
		pthread_cond_signal(&mCond);
		pthread_mutex_unlock(&mMutex);
#endif
	}
}
