#ifndef __PTHREAD_H__
#define __PTHREAD_H__

#ifdef MYTH_OS_UNIX
#include "thread.h"
namespace Myth
{
	void* ThreadFunction(void* arg);
	class CThreadPool;
	class CPThread : public IThread
	{
	public:
		CPThread();
		virtual ~CPThread();

	public:
		/// create thread
		virtual void start();
		/// terminate thread
		virtual void terminate();
		/// wait for exit
		virtual void wait();
		/// suspend thread
		virtual void suspend();
		/// resume thread
		virtual void resume();

	public:
		IJob*				mpJob;			// ��ִ�ж���

	private:
		pthread_t			mThreadID;		// �߳�ID
		pthread_mutex_t		mMutex;			// ������
		pthread_mutexattr_t	mMutexAttr;		// ����������
		pthread_cond_t		mCond;			// ��������
	};
}
#endif

#endif