#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "commontype.h"
#include "workthread.h"
#include "simplelock.h"
#include "array.h"
#include <list>
#define MAX_JOB_ID_NUM	128
#define MAX_THREAD_CAPACITY 32
namespace Myth
{
	
	class CThreadPool
	{
	public:
		typedef CArray<IJob*, MAX_JOB_ID_NUM> JobArray;

	public:
		CThreadPool();
		~CThreadPool();

	public:
		/// ��ʼ���߳�
		bool initThread(int nThreadSize);
		/// ����
		void run();
		/// �ر������߳�
		void terminateAllThread();
		/// �ȴ������߳��˳�
		void waitAllThread();
		//���һ���̵߳��̳߳�
		bool pushBackThread(CThread* pThread);
		//ɾ��һ���߳�
		bool removeThread(THREAD_ID id);
		//�����߳�IDȡ���߳�ָ��
		CThread* getThread(THREAD_ID id);

		/// ����job
		bool pushBackJob(IJob* pJob);
		/// ɾ��job
		void removeJob(IJob* pJob);
		/// ����job
		IJob* popJob();
		/// ���ö�Ӧ��jobΪ����״̬
		void setJobFree(IJob* pJob);
		/// ����job������
		void resetJobIndex();
		/// ����job id�õ���Ӧ��job
		IJob* getJobByID(byte nJobID);

	private:
		/// ���е�JobIndex,����ģ�ⵯ��
		byte				mJobIndex;
		/// �̳߳����λ���������б�����Щslot����Ϊ��)
		char				mThreadMaxIndex;
		/// �߳��б�
		CThread*			mThreadList[MAX_THREAD_CAPACITY];
		/// 
		CSimpleLock			mSimpleLock;
		// Job ID ������
		byte				mJobIDCount;
		// ͨ��Job IDȡ��job
		IJob*				mJobList[MAX_JOB_ID_NUM];
		// Job�б�
		JobArray			mJobArray;

	};
}
#endif