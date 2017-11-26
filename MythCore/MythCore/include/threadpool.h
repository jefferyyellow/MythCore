#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__


#include "commontype.h"
#include "blockmemory.h"
#include "thread.h"
#include "simplelock.h"
#include "array.h"
#include "winthread.h"
#include "linuxthread.h"
#include <list>
namespace Myth
{
	class CThreadPool
	{
	public:
		typedef std::list<IThread*> ThreadList;
		typedef CArray<IJob*, 32> JobArray;

	public:
		CThreadPool(int nThreadSize)
		{
			mJobIndex = 0;
			mIdleListNum = nThreadSize;
			for (int i = 0; i < nThreadSize; ++ i)
			{
				IThread* pThread = NULL;

#ifdef MYTH_OS_WINDOWS
				pThread = new CWinThread;
#else
				pThread = new CPThread;
#endif			

				if (NULL != pThread)
				{
					pThread->setThreadPool(this);
					pThread->setSerialNum(i);
					pThread->start();
					mIdleList.push_back(pThread);
					printf("%x\n", pThread);
				}
			}
			//printf("*************\n");
			//ThreadList::iterator it = mIdleList.begin();

			//int i = 0;
			//IThread* pIdleThread = NULL;
			//for (; it != mIdleList.end(); ++ it)
			//{
			//	printf("%x\n", *it);
			//	printf("%d\n", i);
			//	++ i;
			//	//pIdleThread = *it;
			//}
		}
		~CThreadPool()
		{

		}

	public:
		void				pushBackJob(IJob* pJob)
		{
			mSimpleLock.lock();
			mJobArray.push_back(pJob);
			mSimpleLock.unlock();
		}

		IJob*				popJob()
		{
			mSimpleLock.lock();
			
			if (mJobIndex >= mJobArray.size())
			{
				mSimpleLock.unlock();
				return NULL;
			}
			IJob* pRunJob = NULL;
			for (; mJobIndex < mJobArray.size(); ++ mJobIndex)
			{
				if (!(mJobArray[mJobIndex]->getBusy()))
				{
					pRunJob = mJobArray[mJobIndex];
					mJobArray[mJobIndex]->setBusy(true);
					++mJobIndex;
					break;
				}
			}			
		
			mSimpleLock.unlock();

			return pRunJob;
		}

		void				setJobFree(IJob* pJob)
		{
			mSimpleLock.lock();
			pJob->setBusy(false);
			mSimpleLock.unlock();
		}



		IThread*			popIdleThread()
		{
			mSimpleLock.lock();
			if (mIdleList.empty())
			{
				mSimpleLock.unlock();
				return NULL;
			}
			ThreadList::iterator it = mIdleList.begin();
			IThread* pIdleThread = *it;
			mIdleList.pop_front();
			-- mIdleListNum;
			mSimpleLock.unlock();

			return pIdleThread;
		}

		void				pushIdleThread(IThread* pIdleThread)
		{
			mSimpleLock.lock();
			mIdleList.push_back(pIdleThread);
			//printf("\npushIdleThread: %p\n", pIdleThread);
			if (pIdleThread == (void*)0xcdcdcdcd)
			{
				int i = 0;
			}
			++ mIdleListNum;
			mSimpleLock.unlock();
		}

		void				resetJob()
		{
			mSimpleLock.lock();
			mJobIndex = 0;
			mSimpleLock.unlock();
		}

		void				run();

	private:
		ThreadList			mIdleList;
		byte				mIdleListNum; 

		JobArray			mJobArray;
		byte				mJobIndex;

		CSimpleLock			mSimpleLock;
	};
}
#endif