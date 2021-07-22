#ifndef __THREADEVENT_H__
#define __THREADEVENT_H__
class CThreadEvent
{
public:
	CThreadEvent(bool bManualReset)
	{
#ifdef MYTH_OS_WINDOWS
		mEvent = CreateEvent(nullptr, bManualReset, FALSE, nullptr);
#else
		pthread_mutexattr_init(&mMutexAttr);
		pthread_mutexattr_settype(&mMutexAttr,PTHREAD_MUTEX_RECURSIVE_NP); 
		pthread_mutex_init(&mMutex, &mMutexAttr);
#endif
	}

	~CThreadEvent()
	{
#ifdef MYTH_OS_WINDOWS
		if (nullptr != mEvent)
		{
			CloseHandle(mEvent);
			mEvent = nullptr;
		}
#else
		pthread_cond_destroy(&mCond);
		pthread_mutexattr_destroy(&mMutexAttr);
#endif
	}

	void		wait()
	{
#ifdef MYTH_OS_WINDOWS
		WaitForSingleObject(mEvent, INFINITE);
#else
		pthread_mutex_lock(&mMutex);
		pthread_cond_wait(&mCond, &mMutex);
		pthread_mutex_unlock(&mMutex);
#endif
	}

	void		signal()
	{
#ifdef MYTH_OS_WINDOWS
		SetEvent(mEvent);
#else
		pthread_mutex_lock(&mMutex);
		pthread_cond_signal(&mCond);
		pthread_mutex_unlock(&mMutex);
#endif
	}

	void		broadcast()
	{
#ifdef MYTH_OS_WINDOWS
		SetEvent(mEvent);
#else
		pthread_mutex_lock(&mMutex);
		pthread_cond_broadcast(&mCond);
		pthread_mutex_unlock(&mMutex);
#endif
	}

private:
#ifdef MYTH_OS_WINDOWS
	HANDLE				mEvent;			// 句柄
#else
	pthread_mutex_t		mMutex;			// 互斥体
	pthread_cond_t		mCond;			// 条件变量
	pthread_mutexattr_t	mMutexAttr;		// 互斥体属性
#endif
};
#endif