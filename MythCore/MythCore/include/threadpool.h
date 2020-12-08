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
		/// 初始化线程
		bool initThread(int nThreadSize);
		/// 运行
		void run();
		/// 关闭所有线程
		void terminateAllThread();
		/// 等待所有线程退出
		void waitAllThread();
		//添加一个线程到线程池
		bool pushBackThread(CThread* pThread);
		//删除一个线程
		bool removeThread(THREAD_ID id);
		//根据线程ID取得线程指针
		CThread* getThread(THREAD_ID id);

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
		/// 根据job id得到对应的job
		IJob* getJobByID(byte nJobID);

	private:
		/// 运行的JobIndex,用来模拟弹出
		byte				mJobIndex;
		/// 线程池最大位置索引（列表中有些slot可能为空)
		char				mThreadMaxIndex;
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