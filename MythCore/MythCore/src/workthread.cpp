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
		if (nullptr == pThead)
		{
			return 0;
		}


		pThead->run();
		return 0;
	}

	CThread::CThread()
	{
		mpThreadPool = nullptr;
		mpJob = nullptr;
#ifdef MYTH_OS_WINDOWS
		mThreadHand = INVALID_HANDLE_VALUE;
#else
		pthread_cond_init(&mCond, nullptr);
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
		mThreadHand = ::CreateThread(nullptr, 0, ThreadFunction, (void*)this, 0, (LPDWORD)&mThreadID);
#else
		pthread_create(&mThreadID, nullptr, ThreadFunction, (void *)this);
#endif
	}

	/// 终止线程
	void CThread::terminate()
	{
		setThreadState(emThreadState_Exiting);
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
		setThreadState(emThreadState_Exited);
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

	/// 运行
	void CThread::run()
	{
		setThreadState(emThreadState_Runing);
		while (true)
		{
			if (nullptr == mpJob)
			{
				suspend();
				if (nullptr != mpThreadPool)
				{
					IJob* pJob = mpThreadPool->popJob();
					if (nullptr != pJob)
					{
						mpJob = pJob;
					}
				}
				continue;
			}
			mpJob->doing(getSerialNum());

			if (nullptr != mpThreadPool)
			{
				mpThreadPool->setJobFree(mpJob);
				IJob* pJob = mpThreadPool->popJob();
				mpJob = pJob;
			}
		}
	}
}
