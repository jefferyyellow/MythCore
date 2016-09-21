#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__


#include "commontype.h"
#include "blockmemory.h"
#include "iterator.h"
#include "list.h"
#include "thread.h"
#include "simplelock.h"
#include "array.h"
#include "winthread.h"
namespace Myth
{
	class CThreadPool
	{
	public:
		typedef CList<IThread*, 32, 0> ThreadList;
		typedef CArray<IJob*, 32> JobArray;

	public:
		CThreadPool(int nThreadSize)
		{
			mJobIndex = 0;
			mIdleListNum = nThreadSize;
			for (int i = 0; i < nThreadSize; ++ i)
			{
				CWinThread* pThead = new CWinThread;
				if (NULL != pThead)
				{
					pThead->setThreadPool(this);
					pThead->setSerialNum(i);
					pThead->start();
					mIdleList.push_back(pThead);
					printf("%x\n", pThead);
				}
			}
			printf("*************\n");
			ThreadList::iterator it = mIdleList.begin();

			IThread* pIdleThread = NULL;
			for (; it != mIdleList.end(); ++ it)
			{
				printf("%x\n", *it);
				//pIdleThread = *it;
			}
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

			IJob* pRunJob = mJobArray[mJobIndex];
			++mJobIndex;
		
			mSimpleLock.unlock();

			return pRunJob;
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
		uint8				mIdleListNum; 

		JobArray			mJobArray;
		uint8				mJobIndex;

		CSimpleLock			mSimpleLock;
	};
}
#endif