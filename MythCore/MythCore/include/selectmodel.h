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

		CSelectModel(CTcpSocket* pAllSocket, int nSocketNum)
		{
			init();
			mpAllSocket = pAllSocket;
			mSocketNum = nSocketNum;
		}

		~CSelectModel()
		{
			init();
		}

		void init()
		{
			mMaxFd = ~0;
			FD_ZERO(&mReadSet);
			FD_ZERO(&mReadBackSet);
			mSelectTime.tv_sec = 0;
			mSelectTime.tv_usec = 0;
			mpAllSocket = NULL;
			mSocketNum = 0;
		}

	public:
		void			createListenSocket(char* pIP, uint32 uPort, int nListNum);
		void			selectAllFd();
		void			processRead();
		void			addNewSocket(CTcpSocket* pNewSocket);
		void			removeSocket(CTcpSocket* pRemoveSocket);
		int				findFreeSocketIndex();
		CTcpSocket*		getFreeSocket(int &rSocketIndex);

	public:
		int				getMaxFd(){return mMaxFd;}
		void			setMaxFd(int nMaxFd){mMaxFd = nMaxFd;}

		fd_set			getReadSet(){return mReadSet;}
		void			setReadSet(fd_set tReadSet){mReadSet = tReadSet;}

		fd_set			getReadBackSet(){return mReadBackSet;}
		void			setReadBackSet(fd_set tReadBackSet){mReadBackSet = tReadBackSet;}

		CTcpSocket*		GetAllSocket(){return mpAllSocket;}
		void			SetAllSocket(CTcpSocket* pAllSocket){mpAllSocket = pAllSocket;}

		uint16			GetSocketNum(){return mSocketNum;}
		void			SetSocketNum(uint16 nSocketNum){mSocketNum = nSocketNum;}
	private:
		SOCKET			mMaxFd;
		fd_set			mReadSet;
		fd_set			mReadBackSet;
		timeval			mSelectTime;
		CTcpSocket*		mpAllSocket;
		uint16			mSocketNum;
		uint16			mMaxSocketIndex;
	};

}

#endif