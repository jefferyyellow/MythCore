#ifndef __EPOLLMODEL_H__
#define __EPOLLMODEL_H__
#ifdef MYTH_OS_UNIX
#include "commontype.h"
#include <sys/epoll.h>
#include <stdlib.h>
#include <unistd.h>
namespace Myth
{
	class CTcpSocket;
	class CEpollModel
	{
	public:
		CEpollModel()
		{

		}

		CEpollModel(CTcpSocket* pAllSocket, int nSocketCapacity)
		{
			init();
			mpAllSocket = pAllSocket;
			mSocketCapacity = nSocketCapacity;
		}

		~CEpollModel()
		{
			finial();
		}

		void init()
		{
			mEpollFd = -1;
			mpWaitEvents = NULL;
			memset(&mCtrlEvent, 0, sizeof(epoll_event));
			mpAllSocket = NULL;
			mSocketCapacity = 0;
			mWaitTimeOut = 0;
		}

		void finial()
		{
			if (NULL != mpAllSocket)
			{
				free(mpAllSocket);
				mpAllSocket = NULL;
			}
			close(mEpollFd);
		}

	public:
		int	initEpollSocket();
		int createListenSocket(char* pIP, uint32 uPort, int nListNum, int nSendBuffSize, int nRecvBuffSize);
		int addSocket(int nfd);
		int delSocket(int nfd);
		void epollWait();
	private:
		int                 mEpollFd;
		struct epoll_event  *mpWaitEvents;
		struct epoll_event  mCtrlEvent;
		CTcpSocket*			mpAllSocket;
		uint16				mSocketCapacity;
		int					mWaitTimeOut;
	};
}

#endif //  MYTH_OS_UNIX
#endif // __EPOLLMODEL_H__