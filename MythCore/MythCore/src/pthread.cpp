#include "pthread.h"
#ifdef MYTH_OS_UNIX
namespace Myth
{
	CPThread()
	{
		mpRunnable = NULL;
		mThreadID = 0;
		pthread_cond_init(&mCond, NULL);
		pthread_mutex_init(&mMutex, NULL);
	}

	~CPThread();
	{
		terminate();
	}

	void CPThread::start()
	{
		if (0 == mThreadID)
		{
			return;
		}
		return pthread_create( &mThreadID, NULL, ThreadFunction, (void *)this );
	}

	void CPThread::terminate()
	{
		if (0 == mThreadID)
		{
			return;
		}
		pthread_cancel(mThreadID);
	}

	void CPThread::wait()
	{
		if (0 == mThreadID)
		{
			return;
		}
		int error = pthread_join(mThreadID, 0);
		switch (error)
		{
			case 0:
				break;
			case EINVAL:
			{
				//throw EThread("Thread is not joinable");
				break;
			}
			case ESRCH:
			{
				//throw EThread("No thread found with this id");
				break;
			}
			case EDEADLK:
			{
				//throw EThread("Deadlock detected or calling thread waits for itself");
				break;
			}
			default:
			{
				//throw EThread("Unknown thread join error");
				break;
			}
		}
		//if (_State != ThreadStateFinished)
		//{
		//	throw EThread("Thread did not finish, this should not happen");
		//}
	}

	/// suspend thread
	void suspend()
	{
		if (0 == mThreadID)
		{
			return;
		}
		pthread_mutex_lock(&mMutex);
		pthread_cond_wait(&mCond, &mMutex);
		pthread_mutex_unlock(&mMutex);
	}

	/// resume thread
	void resume()
	{
		if (0 == mThreadID)
		{
			return;
		}

		pthread_mutex_lock(&mMutex);
		pthread_cond_signal(&mCond);
		pthread_mutex_unlock(&mMutex);
	}
}
#endif