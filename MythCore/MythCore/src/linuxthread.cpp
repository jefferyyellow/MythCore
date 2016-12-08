#include "linuxthread.h"
#include "threadpool.h"
#ifdef MYTH_OS_UNIX
namespace Myth
{

	void* ThreadFunction(void* arg)
	{
		CPThread* pThead = (CPThread*)arg;
		if (NULL == pThead)
		{
			return 0;
		}

		pThead->setThreadState(emThreadState_Runing);
		while (true)
		{
			if (NULL == pThead->mpJob)
			{
				pThead->mpThreadPool->pushIdleThread(pThead);
				pThead->suspend();
				if (NULL != pThead->mpThreadPool)
				{
					IJob* pJob = pThead->mpThreadPool->popJob();
					if (NULL != pJob)
					{
						pThead->mpJob = pJob;
						printf("popJob %d\n", pThead->getSerialNum());
					}
				}
				continue;
			}
			pThead->mpJob->doing(pThead->getSerialNum());
			if (NULL != pThead->mpThreadPool)
			{
				IJob* pJob = pThead->mpThreadPool->popJob();
				pThead->mpJob = pJob;
				if (NULL != pJob)
				{
					printf("popJob %d\n", pThead->getSerialNum());
				}

			}
		}

		return 0;
	}
	CPThread::CPThread()
	{
		mpJob = NULL;
		mpThreadPool = NULL;
		mThreadID = 0;
		pthread_cond_init(&mCond, NULL);
		pthread_mutex_init(&mMutex, NULL);
		mThreadState = emThreadState_None;
		mSerialNum = 0;
	}

	CPThread::~CPThread()
	{
		terminate();
	}

	void CPThread::start()
	{
		if (0 != mThreadID)
		{
			return;
		}
		pthread_create( &mThreadID, NULL, ThreadFunction, (void *)this );
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
		//switch (error)
		//{
		//	case 0:
		//		break;
		//	case EINVAL:
		//	{
		//		//throw EThread("Thread is not joinable");
		//		break;
		//	}
		//	case ESRCH:
		//	{
		//		//throw EThread("No thread found with this id");
		//		break;
		//	}
		//	case EDEADLK:
		//	{
		//		//throw EThread("Deadlock detected or calling thread waits for itself");
		//		break;
		//	}
		//	default:
		//	{
		//		//throw EThread("Unknown thread join error");
		//		break;
		//	}
		//}
		//if (_State != ThreadStateFinished)
		//{
		//	throw EThread("Thread did not finish, this should not happen");
		//}
	}

	/// suspend thread
	void CPThread::suspend()
	{
		if (0 == mThreadID)
		{
			return;
		}
		pthread_mutex_lock(&mMutex);
		pthread_cond_wait(&mCond, &mMutex);
		pthread_mutex_unlock(&mMutex);
		setThreadState(emThreadState_Suspend);
	}

	/// resume thread
	void CPThread::resume()
	{
		if (0 == mThreadID)
		{
			return;
		}

		pthread_mutex_lock(&mMutex);
		pthread_cond_signal(&mCond);
		pthread_mutex_unlock(&mMutex);
		setThreadState(emThreadState_Runing);
	}
}
#endif