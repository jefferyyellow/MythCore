#include "threadpool.h"
#include "workthread.h"
namespace Myth
{
	CThreadPool::CThreadPool()
	{
		mJobIndex = 0;
		mThreadNum = 0;
		for (int i = 0; i < MAX_THREAD_CAPACITY; ++ i)
		{
			mThreadList[i] = NULL;
		}
		mJobIDCount = 0;
		for (int i = 0; i < MAX_JOB_ID_NUM; ++ i)
		{
			mJobList[i] = NULL;
		}
	}

	CThreadPool::~CThreadPool()
	{
		terminateAllThread();
		waitAllThread();
	}

	/// ��ʼ���߳�
	bool CThreadPool::initThread(int nThreadSize)
	{
		if (nThreadSize > MAX_THREAD_CAPACITY)
		{
			return false;
		}
		mJobIndex = 0;
		mThreadNum = nThreadSize;
		for (int i = 0; i < nThreadSize; ++i)
		{
			mThreadList[i] = new CThread;
			if (NULL == mThreadList[i])
			{
				terminateAllThread();
				waitAllThread();
				return false;
			}

			mThreadList[i]->setThreadPool(this);
			mThreadList[i]->setSerialNum(i);
			mThreadList[i]->start();
		}
		return true;
	}

	/// ����job
	bool CThreadPool::pushBackJob(IJob* pJob)
	{
		if (NULL == pJob)
		{
			return false;
		}
		CAutoLock tLock(&mSimpleLock);
		// Job�б��Լ�����
		if (mJobArray.size() >= mJobArray.capacity())
		{
			return false;
		}
		mJobArray.push_back(pJob);
		for (int i = 0; i < MAX_JOB_ID_NUM; ++i)
		{
			if (NULL == mJobList[i])
			{
				pJob->setJobID(i);
				mJobList[i] = pJob;
				break;
			}
		}

		return true;
	}

	/// ɾ��job
	void CThreadPool::removeJob(IJob* pJob)
	{
		if (NULL == pJob)
		{
			return;
		}

		CAutoLock tLock(&mSimpleLock);
		for (JobArray::iterator it = mJobArray.begin(); it != mJobArray.end(); ++it)
		{
			if (pJob == *it)
			{
				mJobArray.erase(it);
				break;
			}
		}
		if (pJob->getJobID() >= 0 && pJob->getJobID() < MAX_JOB_ID_NUM)
		{
			mJobList[pJob->getJobID()] = NULL;
		}
		else
		{
			for (int i = 0; i < MAX_JOB_ID_NUM; ++i)
			{
				if (pJob == mJobList[i])
				{
					mJobList[i] = NULL;
					break;
				}
			}
		}
	}

	/// ����job
	IJob* CThreadPool::popJob()
	{
		CAutoLock tLock(&mSimpleLock);

		if (mJobIndex >= mJobArray.size())
		{
			return NULL;
		}
		IJob* pRunJob = NULL;
		for (; mJobIndex < mJobArray.size(); ++mJobIndex)
		{
			if (!(mJobArray[mJobIndex]->getBusy()))
			{
				pRunJob = mJobArray[mJobIndex];
				mJobArray[mJobIndex]->setBusy(true);
				++mJobIndex;
				break;
			}
		}

		return pRunJob;
	}

	/// ���ö�Ӧ��jobΪ����״̬
	void CThreadPool::setJobFree(IJob* pJob)
	{
		CAutoLock tLock(&mSimpleLock);
		pJob->setBusy(false);
	}

	/// ����job������
	void CThreadPool::resetJobIndex()
	{
		CAutoLock tLock(&mSimpleLock);
		mJobIndex = 0;
	}
	
	/// ����
	void CThreadPool::run()
	{
		CAutoLock tLock(&mSimpleLock);
		if (mJobIndex >= mJobArray.size())
		{
			mJobIndex = 0;
		}
		
		for (int i = 0; i < mThreadNum; ++ i)
		{
			mThreadList[i]->resume();
		}
	}

	/// �ر������߳�
	void CThreadPool::terminateAllThread()
	{
		for (int i = 0; i < mThreadNum; ++i)
		{
			mThreadList[i]->terminate();
		}
	}

	/// �ȴ������߳��˳�
	void CThreadPool::waitAllThread()
	{
		for (int i = 0; i < mThreadNum; ++i)
		{
			mThreadList[i]->wait();
		}
	}

	/// ����job id�õ���Ӧ��job
	IJob* CThreadPool::getJobByID(byte nJobID)
	{
		if (nJobID < 0 || nJobID >= MAX_JOB_ID_NUM)
		{
			return NULL;
		}
		return mJobList[nJobID];
	}
}
