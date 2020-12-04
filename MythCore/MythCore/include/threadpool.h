#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "commontype.h"
#include "thread.h"
#include "simplelock.h"
#include "array.h"
#include <list>
#define MAX_JOB_ID_NUM	128
#define MAX_THREAD_CAPACITY 32
namespace Myth
{
	class CThread;
	class CThreadPool
	{
	public:
		typedef CArray<IJob*, MAX_JOB_ID_NUM> JobArray;

	public:
		CThreadPool();
		~CThreadPool();

	public:
		/// 初始化线程
		bool initThread(int nThreadSize);
		/// 运行
		void run();
		/// 增加job
		bool pushBackJob(IJob* pJob);
		/// 删除job
		void removeJob(IJob* pJob);
		/// 弹出job
		IJob* popJob();
		/// 设置对应的job为空闲状态
		void setJobFree(IJob* pJob);
		/// 重置job的索引
		void resetJobIndex();
		/// 关闭所有线程
		void terminateAllThread();
		/// 等待所有线程退出
		void waitAllThread();
		/// 根据job id得到对应的job
		IJob* getJobByID(byte nJobID);

	private:
		/// 运行的JobIndex,用来模拟弹出
		byte				mJobIndex;
		/// 线程池的线程数量
		byte				mThreadNum;
		/// 线程列表
		CThread*			mThreadList[MAX_THREAD_CAPACITY];
		/// 
		CSimpleLock			mSimpleLock;
		// Job ID 生成器
		byte				mJobIDCount;
		// 通过Job ID取出job
		IJob*				mJobList[MAX_JOB_ID_NUM];
		// Job列表
		JobArray			mJobArray;

	};
}
#endif