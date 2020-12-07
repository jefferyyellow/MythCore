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
			mBusy = false;
			mJobType = 0;
			mJobID = 0;
		}
		virtual ~IJob()
		{
		}

	public:
		virtual void doing(int uParam) = 0;

		byte getJobType() const { return mJobType; }
		void setJobType(byte nValue) { mJobType = nValue; }

		bool getBusy(){return mBusy;}
		void setBusy(bool bBusy){mBusy = bBusy;}

		byte getJobID() const { return mJobID; }
		void setJobID(byte nValue) { mJobID = nValue; }

	private:
		byte mJobType;
		byte mJobID;
		bool mBusy;
	};


	enum EmThreadState
	{
		emThreadState_None		= 0,	// 非法状态
		emThreadState_Ready		= 1,	// 准备完成
		emThreadState_Runing	= 2,	// 运行中
		emThreadState_Exiting	= 3,	// 退出中
		emThreadState_Exited	= 4,	// 退出完成
		emThreadStateMax
	};

#ifdef MYTH_OS_WINDOWS
	typedef DWORD		THREAD_ID;
#else 
	typedef pthread_t	THREAD_ID;
#endif
	class CThreadPool;
	class IThread
	{
	public:
		IThread()
		{
			mThreadState  = emThreadState_Ready;
			mSerialNum = 0;
			mThreadID = 0;
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

	public:
		EmThreadState getThreadState(){ return mThreadState; }
		void setThreadState(EmThreadState eThreadState){ mThreadState = eThreadState; }

		int getSerialNum(){ return mSerialNum; }
		void setSerialNum(int nSerialNum){ mSerialNum = nSerialNum; }

		THREAD_ID getThreadID() const { return mThreadID; }
		void setThreadID(THREAD_ID nValue) { mThreadID = nValue; }
	protected:
		//  线程状态
		EmThreadState		mThreadState;
		// 线程序列号
		int					mSerialNum;
		// 线程ID
		THREAD_ID			mThreadID;
	};
}
#endif