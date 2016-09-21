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
		emThreadState_None			= 0,	// ��״̬
		emThreadState_Initing		= 1,	// ��ʼ��
		emThreadState_inited		= 2,	// ��ʼ�����
		emThreadState_Runing		= 3,	// ������
		emThreadState_Suspend		= 4,	// ��ͣ��
		emThreadState_Exiting		= 5,	// �˳���
		emThreadState_Exited		= 6,	// �˳����
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
		IJob*			mpJob;			// ��ִ�ж���
		CThreadPool*	mpThreadPool;	// �̳߳�

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