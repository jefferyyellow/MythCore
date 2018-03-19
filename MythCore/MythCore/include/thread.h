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
		}
		virtual ~IJob()
		{

		}

	public:
		virtual void doing(int uParam) = 0;

		byte	getJobID(){return mJobID;}
		void	setJobID(byte mJobID){mJobID = mJobID;}

		bool	getBusy(){return mBusy;}
		void	setBusy(bool bBusy){mBusy = bBusy;}
	private:
		byte mJobID;
		bool mBusy;
	};


	enum EmThreadState
	{
		emThreadState_None		= 0,	// ��״̬
		emThreadState_Initing	= 1,	// ��ʼ��
		emThreadState_inited	= 2,	// ��ʼ�����
		emThreadState_Runing	= 3,	// ������
		emThreadState_Suspend	= 4,	// ��ͣ��
		emThreadState_Exiting	= 5,	// �˳���
		emThreadState_Exited	= 6,	// �˳����
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

		virtual EmThreadState getThreadState() = 0;
		virtual void setThreadState(EmThreadState eThreadState) = 0;

		virtual CThreadPool* getThreadPool() = 0;
		virtual void setThreadPool(CThreadPool* pThreadPool) = 0;

		virtual int getSerialNum() = 0;
		virtual void setSerialNum(int nSerialNum) = 0;
	};
}
#endif