#ifndef __SELECTMODEL_H__
#define __SELECTMODEL_H__
#include "commontype.h"
#include "tcpsocket.h"
namespace Myth
{
	class CSelectModel
	{
	public:
		CSelectModel()
		{
			init();
		}

		CSelectModel(CBuffTcpSocket* pAllSocket, int nSocketCapacity)
		{
			init();
			mpAllSocket = pAllSocket;
			mSocketCapacity = nSocketCapacity;
		}

		~CSelectModel()
		{
			init();
		}

		void init()
		{
#ifdef MYTH_OS_WINDOWS
			mMaxFd = 0;
#else
			mMaxFd = -1;
#endif // DEBUG
			FD_ZERO(&mReadSet);
			FD_ZERO(&mReadBackSet);
			mSelectTime.tv_sec = 0;
			mSelectTime.tv_usec = 0;
			mpAllSocket = NULL;
			mSocketCapacity = 0;
			mMaxSocketIndex = 0;
		}
	public:
		static			int initSocketSystem();

	public:
		CBuffTcpSocket*	createListenSocket(char* pIP, uint32 uPort, int nListNum);
		void			selectAllFd();
		void			processRead();
		void			processWrite(int nSocketIndex, char* pBuffer, int nBuffSize);
		void			addNewSocket(CBuffTcpSocket* pNewSocket);
		void			removeSocket(SOCKET nRemoveFd);
		int				findFreeSocketIndex();
		CBuffTcpSocket*	getFreeSocket(int &rSocketIndex);

	public:
		int				getMaxFd(){return mMaxFd;}
		void			setMaxFd(int nMaxFd){mMaxFd = nMaxFd;}

		fd_set			getReadSet(){return mReadSet;}
		void			setReadSet(fd_set tReadSet){mReadSet = tReadSet;}

		fd_set			getReadBackSet(){return mReadBackSet;}
		void			setReadBackSet(fd_set tReadBackSet){mReadBackSet = tReadBackSet;}

		CBuffTcpSocket*		GetAllSocket(){return mpAllSocket;}
		void			SetAllSocket(CBuffTcpSocket* pAllSocket){mpAllSocket = pAllSocket;}

		uint16			GetSocketCapacity(){ return mSocketCapacity; }
		void			SetSocketCapacity(uint16 nSocketNum){ mSocketCapacity = nSocketNum; }
	private:
		SOCKET				mMaxFd;
		fd_set				mReadSet;
		fd_set				mReadBackSet;
		timeval				mSelectTime;
		CBuffTcpSocket*		mpAllSocket;
		uint16				mSocketCapacity;
		uint16				mMaxSocketIndex;
	};

}

#endif