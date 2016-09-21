#include "threadpool.h"

namespace Myth
{
	void CThreadPool::run()
	{
		mSimpleLock.lock();
		mJobIndex = 0;
		int nSize = mJobArray.size() > mIdleListNum ? mIdleListNum : mJobArray.size();
		for (int i = 0; i < nSize; ++ i)
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