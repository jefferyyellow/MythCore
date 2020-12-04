#ifndef __WINTHREAD_H__
#define __WINTHREAD_H__
#ifdef MYTH_OS_WINDOWS

#include "commontype.h"
#include "thread.h"
namespace Myth
{
	unsigned long __stdcall ThreadFunction(void* arg);

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
		IJob*			mpJob;			// 可执行对象

	private:
		int				mThreadID;
		HANDLE			mThreadHand;
	};
}
#endif
#endif