#ifndef __WORKTHREAD_H__
#define __WORKTHREAD_H__
#include "commontype.h"
#include "thread.h"
namespace Myth
{
#ifdef MYTH_OS_WINDOWS
	DWORD __stdcall ThreadFunction(void* arg);
	typedef DWORD		THREAD_ID;
#else 
	void* ThreadFunction(void* arg);
	typedef pthread_t	THREAD_ID;
#endif

	class CThread : public IThread
	{
	public:
		CThread();
		virtual ~CThread();

	public:
		/// 开始（创建）线程
		virtual void start();
		/// 终止线程
		virtual void terminate();
		/// 等待退出
		virtual void wait();
		/// 暂停线程
		virtual void suspend();
		/// 唤醒线程
		virtual void resume();


	public:
		CThreadPool* getThreadPool(){ return mpThreadPool; }
		void setThreadPool(CThreadPool* pThreadPool){ mpThreadPool = pThreadPool; }
		
		IJob* getMpJob() const { return mpJob; }
		void setMpJob(IJob* nValue) { mpJob = nValue; }

	public:
		// 归属的线程池
		CThreadPool*		mpThreadPool;
		// 正在执行的job
		IJob*				mpJob;
	private:
		// 线程ID
		THREAD_ID			mThreadID;
#ifdef MYTH_OS_WINDOWS
		// windows线程句柄
		HANDLE				mThreadHand;
#else
		// 包互斥体及互斥体属性，用于保护条件变量
		pthread_mutex_t		mMutex;			
		pthread_mutexattr_t	mMutexAttr;		
		// 条件变量
		pthread_cond_t		mCond;			
#endif
	};
}


#endif