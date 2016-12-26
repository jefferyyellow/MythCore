#ifndef __THREAD_H__
#define __THREAD_H__
#include "commontype.h"
namespace Myth
{

	class IJob
	{
	public:
		IJob()
		{

		}
		virtual ~IJob()
		{

		}

	public:
		virtual void doing(uint32 uParam) = 0;

		uint8	getJobID(){return mJobID;}
		void	setJobID(uint8 mJobID){mJobID = mJobID;}
	private:
		uint8 mJobID;
	};


	enum TmThreadState
	{
		emThreadState_None		= 0,	// 无状态
		emThreadState_Initing	= 1,	// 初始中
		emThreadState_inited	= 2,	// 初始化完成
		emThreadState_Runing	= 3,	// 运行中
		emThreadState_Suspend	= 4,	// 暂停中
		emThreadState_Exiting	= 5,	// 退出中
		emThreadState_Exited	= 6,	// 退出完成
		emThreadStateMax
	};
	class CThreadPool;
	class IThread
	{
	public:
		IThread()
		{

		}
		virtual ~IThread()
		{

		}

	public:
		/// create thread
		virtual void start() = 0;
		/// terminate thread
		virtual void terminate() = 0;
		/// wait for exit
		virtual void wait() = 0;
		/// suspend thread
		virtual void suspend() = 0;
		/// resume thread
		virtual void resume() = 0;

		virtual TmThreadState getThreadState() = 0;
		virtual void setThreadState(TmThreadState eThreadState) = 0;

		virtual CThreadPool* getThreadPool() = 0;
		virtual void setThreadPool(CThreadPool* pThreadPool) = 0;

		virtual sint getSerialNum() = 0;
		virtual void setSerialNum(sint nSerialNum) = 0;
	};
}
#endif