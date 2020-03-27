#ifndef __SELECTSERVER_H__
#define __SELECTSERVER_H__
#include "commontype.h"
#include "selectmodel.h"

namespace Myth
{
	typedef void (*SrvReceiveMsgHandle)(CTcpSocket* pSocket);
	typedef void (*SrvNewConnHandle)(CTcpSocket* pSocket);
	typedef void (*SrvRemoveConnHandle)(CTcpSocket* pSocket);
	class CSelectServer
	{
	public:
		CSelectServer(SrvReceiveMsgHandle tReceiveMsgHanle,SrvNewConnHandle tNewConnHandle, SrvRemoveConnHandle tRemoveConnHandle)
			:mReceiveMsgHandle(tRemoveConnHandle), mNewConnHandle(tNewConnHandle), mRemoveConnHandle(tRemoveConnHandle)
		{
			mTcpSocket = NULL;
			mSelectModel = NULL;
			mCacheSize = 0;
			mMaxSocketNum = 0;
		}
		~CSelectServer()
		{
			clear();
		}

		bool	initSocket(int nMaxSocketNum, int nCacheSize, int* pListenPort, int nPortNum, int nBackLogNum);
		void	clear();
		void	receiveMessage();
		int		sendMessage(CTcpSocket* pSocket, const byte* pBuff, int nBuffSize);
		/// ���socket
		void	clearSocket(CTcpSocket* pSocket);

	private:
		// socket
		CTcpSocket*				mTcpSocket;
		// Selectģ��
		CSelectModel*			mSelectModel;
		int						mCacheSize;
		int						mMaxSocketNum;
		// �յ���Ϣ�Ĵ���
		SrvReceiveMsgHandle		mReceiveMsgHandle;
		// �µ����ӵ����Ĵ���
		SrvNewConnHandle		mNewConnHandle;
		// ɾ�����ӵĴ���
		SrvRemoveConnHandle		mRemoveConnHandle;
	};
}
#endif // !__SELECTSERVER_H__

