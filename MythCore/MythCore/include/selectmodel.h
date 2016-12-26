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

		CSelectModel(CTcpSocket* pAllSocket, int nSocketCapacity)
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
		CTcpSocket*		createListenSocket(char* pIP, uint32 uPort, int nListNum);
		void			selectAllFd();
		//void			processRead();
		int				processWrite(int nSocketIndex, char* pBuffer, int nBuffSize);
		void			addNewSocket(CTcpSocket* pNewSocket);
		void			removeSocket(SOCKET nRemoveFd);
		int				findFreeSocketIndex();
		CTcpSocket*		getFreeSocket(int &rSocketIndex);
		CTcpSocket*		getSocket(int nIndex)
		{
			if (nIndex < 0 || nIndex >= mSocketCapacity)
			{
				return NULL;
			}
			return &(mpAllSocket[nIndex]);
		}

	public:
		int				getMaxFd(){return mMaxFd;}
		void			setMaxFd(int nMaxFd){mMaxFd = nMaxFd;}

		fd_set&			getReadSet(){return mReadSet;}
		void			setReadSet(fd_set& tReadSet){mReadSet = tReadSet;}

		//fd_set&			getReadBackSet(){return mReadBackSet;}
		//void			setReadBackSet(fd_set& tReadBackSet){mReadBackSet = tReadBackSet;}

		CTcpSocket*		getAllSocket(){return mpAllSocket;}
		void			setAllSocket(CTcpSocket* pAllSocket){mpAllSocket = pAllSocket;}

		uint16			getSocketCapacity(){ return mSocketCapacity; }
		void			setSocketCapacity(uint16 nSocketNum){ mSocketCapacity = nSocketNum; }

		uint16			getMaxSocketIndex(){return mMaxSocketIndex;}
		void			setMaxSocketIndex(int nMaxSocketIndex){mMaxSocketIndex = nMaxSocketIndex;}
	private:
		SOCKET				mMaxFd;
		fd_set				mReadSet;
		fd_set				mReadBackSet;
		timeval				mSelectTime;
		CTcpSocket*			mpAllSocket;
		uint16				mSocketCapacity;
		uint16				mMaxSocketIndex;
	};

}

#endif