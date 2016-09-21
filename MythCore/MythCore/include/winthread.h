#ifndef __WINTHREAD_H__
#define __WINTHREAD_H__
#ifdef MYTH_OS_WINDOWS

#include "commontype.h"
#include "thread.h"
namespace Myth
{
	unsigned long __stdcall ThreadFunction(void* arg);

	enum TmThreadState
	{
		emThreadState_None			= 0,	// 无状态
		emThreadState_Initing		= 1,	// 初始中
		emThreadState_inited		= 2,	// 初始化完成
		emThreadState_Runing		= 3,	// 运行中
		emThreadState_Suspend		= 4,	// 暂停中
		emThreadState_Exiting		= 5,	// 退出中
		emThreadState_Exited		= 6,	// 退出完成
		emThreadStateMax
	};

	class CThreadPool;
	class CWinThread : public IThread
	{
	public:
		CWinThread();
		virtual ~CWinThread();

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
		/// run thread
		void run();

	public:
		TmThreadState	getThreadState(){ return mThreadState; }
		void			setThreadState(TmThreadState eThreadState){ mThreadState = eThreadState; }

		CThreadPool*	getThreadPool(){ return mpThreadPool; }
		void			setThreadPool(CThreadPool* pThreadPool){ mpThreadPool = pThreadPool; }

		sint			getSerialNum(){ return mSerialNum; }
		void			setSerialNum(sint nSerialNum){ mSerialNum = nSerialNum; }

	public:
		IJob*			mpJob;			// 可执行对象
		CThreadPool*	mpThreadPool;	// 线程池

	private:
		uint32			mThreadID;
		HANDLE			mThreadHand;
		// thread state 
		TmThreadState	mThreadState;
		// thread serial num
		sint			mSerialNum;
	};
}
#endif
#endif