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
		/// ��ʼ���������߳�
		virtual void start();
		/// ��ֹ�߳�
		virtual void terminate();
		/// �ȴ��˳�
		virtual void wait();
		/// ��ͣ�߳�
		virtual void suspend();
		/// �����߳�
		virtual void resume();


	public:
		CThreadPool* getThreadPool(){ return mpThreadPool; }
		void setThreadPool(CThreadPool* pThreadPool){ mpThreadPool = pThreadPool; }
		
		IJob* getMpJob() const { return mpJob; }
		void setMpJob(IJob* nValue) { mpJob = nValue; }

	public:
		// �������̳߳�
		CThreadPool*		mpThreadPool;
		// ����ִ�е�job
		IJob*				mpJob;
	private:
		// �߳�ID
		THREAD_ID			mThreadID;
#ifdef MYTH_OS_WINDOWS
		// windows�߳̾��
		HANDLE				mThreadHand;
#else
		// �������弰���������ԣ����ڱ�����������
		pthread_mutex_t		mMutex;			
		pthread_mutexattr_t	mMutexAttr;		
		// ��������
		pthread_cond_t		mCond;			
#endif
	};
}


#endif