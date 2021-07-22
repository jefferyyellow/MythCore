#ifndef __SELECTCLIENT_H__
#define __SELECTCLIENT_H__
#include "selectmodel.h"
namespace Myth
{
	typedef void (*ClientConnectHandle)();
	typedef void (*ClientDisconnectHandle)();
	typedef void (*ClientReceiveMsgHandle)(CTcpSocket* pSocket);
	class CSelectClient
	{
	public:
		CSelectClient(ClientReceiveMsgHandle ReceiveMsgHandle, ClientConnectHandle tConnect, ClientDisconnectHandle tDisconnect)
			:mReceiveMsgHandle(ReceiveMsgHandle), mConnectHandle(tConnect), mDisconnectHandle(tDisconnect)
		{
			mRecvDataBuffer = nullptr;
		}
		~CSelectClient()
		{
		}
	public:
		bool					initSocket(const char* pServerIP, int nServerPort, int nRecvBufSize);
		void					clear();
		void					receiveMessage();
		int						sendMessage(const byte* pBuff, int nBuffSize);
		/// Çå³ýsocket
		void					clearSocket();
	private:
		CTcpSocket				mTcpSocket;
		CSelectModel			mSelectModel;
		byte*					mRecvDataBuffer;
		ClientReceiveMsgHandle	mReceiveMsgHandle;
		ClientConnectHandle		mConnectHandle;
		ClientDisconnectHandle	mDisconnectHandle;
	};
};

#endif