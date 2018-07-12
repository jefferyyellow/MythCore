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
		
		typedef CArray<IJob*, 32> JobArray;

	public:
		CThreadPool()
		{
			mThreadNum = 0;
			mJobIndex = 0;
		}
		~CThreadPool()
		{
			for (int i = 0; i < 32; ++ i)
			{
				if (NULL != mThreadList[i])
				{
					delete mThreadList[i];
					mThreadList[i] = NULL;
				}
			}
		}

		void				init(int nThreadSize)
		{
			mJobIndex = 0;
			mThreadNum = nThreadSize;
			for (int i = 0; i < nThreadSize; ++i)
			{
#ifdef MYTH_OS_WINDOWS
				mThreadList[i] = new CWinThread;
#else
				mThreadList[i] = new CPThread;
#endif			

				if (NULL != mThreadList[i])
				{
					mThreadList[i]->setThreadPool(this);
					mThreadList[i]->setSerialNum(i);
					mThreadList[i]->start();
					printf("%x\n", mThreadList[i]);
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

		void				resetJob()
		{
			mSimpleLock.lock();
			mJobIndex = 0;
			mSimpleLock.unlock();
		}

		void				run();
		void				terminateAllThread();

	private:
		byte				mJobIndex;
		byte				mThreadNum;
#ifdef MYTH_OS_WINDOWS
		CWinThread*			mThreadList[32];
#else
		CPThread*			mThreadList[32];
#endif
		JobArray			mJobArray;
		CSimpleLock			mSimpleLock;
	};
}
#endif