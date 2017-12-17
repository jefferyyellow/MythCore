#include "threadpool.h"

namespace Myth
{
	void CThreadPool::run()
	{
		mSimpleLock.lock();
		mJobIndex = 0;
		for (int i = 0; i < mThreadNum; ++ i)
		{
			IThread* pThread = popIdleThread();
			if (NULL != pThread)
			{
				pThread->resume();
			}
		}
		mSimpleLock.unlock();
	}

}