#include "threadpool.h"

namespace Myth
{
	void CThreadPool::run()
	{
		//printf("thread pool begin");
		mSimpleLock.lock();
		if (mJobIndex >= mJobArray.size())
		{
			mJobIndex = 0;
		}
		
		for (int i = 0; i < mThreadNum; ++ i)
		{
			mThreadList[i]->resume();
		}
		mSimpleLock.unlock();
	}

}