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

		virtual EmThreadState getThreadState(){ return mThreadState; }
		virtual void setThreadState(EmThreadState eThreadState){ mThreadState = eThreadState; }

		virtual CThreadPool* getThreadPool(){ return mpThreadPool; }
		virtual void setThreadPool(CThreadPool* pThreadPool){ mpThreadPool = pThreadPool; }

		virtual int getSerialNum(){ return mSerialNum; }
		virtual void setSerialNum(int nSerialNum){ mSerialNum = nSerialNum; }

	public:
		IJob*				mpJob;			// ��ִ�ж���
		CThreadPool*		mpThreadPool;	// �̳߳�

	private:
		pthread_t			mThreadID;		// �߳�ID
		pthread_mutex_t		mMutex;			// ������
		pthread_mutexattr_t	mMutexAttr;		// ����������
		pthread_cond_t		mCond;			// ��������

		// thread state 
		EmThreadState		mThreadState;
		// thread serial num
		int					mSerialNum;
	};
}
#endif

#endif