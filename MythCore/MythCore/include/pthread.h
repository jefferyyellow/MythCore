#ifndef __PTHREAD_H__
#define __PTHREAD_H__

#ifdef MYTH_OS_UNIX
#include "thread.h"
namespace Myth
{
	void* ThreadFunction(void* arg);
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
		virtual void suspend() = 0;
		/// resume thread
		virtual void resume() = 0;

	public:
		/// run thread
		void run();

	public:
		IRunnable*			mpRunnable;		// 可执行对象

	private:
		pthread_t			mThreadID;		// 线程ID
		pthread_mutex_t		mMutex;			// 互斥体
		pthread_cond_t		mCond;			// 条件变量
	};
}
#endif

#endif