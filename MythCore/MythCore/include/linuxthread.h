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

		virtual TmThreadState getThreadState(){ return mThreadState; }
		virtual void setThreadState(TmThreadState eThreadState){ mThreadState = eThreadState; }

		virtual CThreadPool* getThreadPool(){ return mpThreadPool; }
		virtual void setThreadPool(CThreadPool* pThreadPool){ mpThreadPool = pThreadPool; }

		virtual sint getSerialNum(){ return mSerialNum; }
		virtual void setSerialNum(sint nSerialNum){ mSerialNum = nSerialNum; }

	public:
		IJob*				mpJob;			// 可执行对象
		CThreadPool*		mpThreadPool;	// 线程池

	private:
		pthread_t			mThreadID;		// 线程ID
		pthread_mutex_t		mMutex;			// 互斥体
		pthread_cond_t		mCond;			// 条件变量

		// thread state 
		TmThreadState		mThreadState;
		// thread serial num
		sint				mSerialNum;
	};
}
#endif

#endif