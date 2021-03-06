#include "winthread.h"
#include "threadpool.h"
#ifdef MYTH_OS_WINDOWS

#include <windows.h>
namespace Myth
{
	unsigned long __stdcall ThreadFunction(void* arg)
	{
		printf("ThreadFunction\n");
		CWinThread* pThead = (CWinThread*)arg;
		if (NULL == pThead)
		{
			return 0;
		}

		pThead->setThreadState(emThreadState_Runing);
		while (true)
		{
			if (NULL == pThead->mpJob)
			{
				pThead->suspend();
				//printf("%d\n", pThead->getSerialNum());
				if (NULL != pThead->mpThreadPool)
				{
					IJob* pJob = pThead->mpThreadPool->popJob();
					if (NULL != pJob)
					{
						pThead->mpJob = pJob;
					}
				}
				continue;
			}
			pThead->mpJob->doing(pThead->getSerialNum());
			
			if (NULL != pThead->mpThreadPool)
			{
				pThead->mpThreadPool->setJobFree(pThead->mpJob);
				IJob* pJob = pThead->mpThreadPool->popJob();
				pThead->mpJob = pJob;
			}
		}

		return 0;
	}


	CWinThread::CWinThread()
	{
		mpJob = NULL;
		mpThreadPool = NULL;
		mThreadID = 0;
		mThreadHand = INVALID_HANDLE_VALUE;
		mThreadState = emThreadState_None;
		mSerialNum = 0;
	}

	CWinThread::~CWinThread()
	{
		if (mThreadHand != INVALID_HANDLE_VALUE)
		{
			TerminateThread(mThreadHand, 0);
			CloseHandle(mThreadHand);
		}

		mThreadHand = INVALID_HANDLE_VALUE;
		mpThreadPool = NULL;
		mThreadID = 0;
		mThreadState = emThreadState_None;
		mSerialNum = 0;
		mpJob = NULL;
	}

	/// create thread
	void CWinThread::start()
	{
		mThreadHand = ::CreateThread(NULL, 0, ThreadFunction, (void*)this, 0, (LPDWORD)&mThreadID);
		if (INVALID_HANDLE_VALUE == mThreadHand)
		{
			return;
		}
		return ;
	}

	/// terminate thread
	void CWinThread::terminate()
	{
		if (mThreadHand == INVALID_HANDLE_VALUE)
		{
			return;
		}

		TerminateThread(mThreadHand, 0);
		CloseHandle(mThreadHand);
		mThreadHand = INVALID_HANDLE_VALUE;
	}

	/// wait for exit
	void CWinThread::wait()
	{
		if (mThreadHand == INVALID_HANDLE_VALUE)
		{
			return;
		}

		WaitForSingleObject(mThreadHand, INFINITE);
		CloseHandle(mThreadHand);
		mThreadHand = INVALID_HANDLE_VALUE;
	}

	/// suspend thread
	void CWinThread::suspend()
	{
		if (mThreadHand == INVALID_HANDLE_VALUE)
		{
			return;
		}
		::SuspendThread(mThreadHand);
	}

	/// resume thread
	void CWinThread::resume()
	{
		if (mThreadHand == INVALID_HANDLE_VALUE)
		{
			return;
		}
		::ResumeThread(mThreadHand) - 1;
	}
}
#endif