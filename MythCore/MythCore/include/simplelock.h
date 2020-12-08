#ifndef __SIMPLELOCK_H__
#define __SIMPLELOCK_H__
#include "commontype.h"
#ifdef MYTH_OS_UNIX
#include <pthread.h>
#endif
namespace Myth
{
	/// 简单锁，windows是关键代码段实现，linux用互斥体
	class CSimpleLock
	{
	public:
		CSimpleLock()
		{
#ifdef MYTH_OS_WINDOWS
			::InitializeCriticalSection(&mCriticalSection);
#else
			pthread_mutexattr_init(&mMutexAttr);
			pthread_mutexattr_settype(&mMutexAttr,PTHREAD_MUTEX_RECURSIVE_NP); 
			pthread_mutex_init(&mMutex, &mMutexAttr);
#endif
		}

		~CSimpleLock()
		{
#ifdef MYTH_OS_WINDOWS
			::DeleteCriticalSection(&mCriticalSection);
#else
			pthread_mutex_destroy(&mMutex);
			pthread_mutexattr_destroy(&mMutexAttr);
#endif
		}

	public:
		void lock()
		{
#ifdef MYTH_OS_WINDOWS
			::EnterCriticalSection(&mCriticalSection);
#else
			pthread_mutex_lock(&mMutex);
#endif
		}
		void unlock()
		{
#ifdef MYTH_OS_WINDOWS
			::LeaveCriticalSection(&mCriticalSection);
#else
			pthread_mutex_unlock(&mMutex);
#endif
		}

	private:
#ifdef MYTH_OS_WINDOWS
		CRITICAL_SECTION		mCriticalSection;
#else
		pthread_mutex_t			mMutex;
		pthread_mutexattr_t		mMutexAttr;
#endif
	};

	/// 自己解锁的辅助结构
	class CAutoLock
	{
	public:
		CAutoLock(CSimpleLock& rSimpleLock):
			mLock(rSimpleLock)
		{
			mLock.lock();
		}
		~CAutoLock()
		{
			mLock.unlock();
		}

	private:
		CSimpleLock& mLock;
	};
}
#endif