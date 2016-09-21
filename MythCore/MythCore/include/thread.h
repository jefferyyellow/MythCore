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
	};

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
	};
}
#endif