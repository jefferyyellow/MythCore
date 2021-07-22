#include "selectclient.h"
namespace Myth
{
	bool CSelectClient::initSocket(const char* pServerIP, int nServerPort, int nRecvBufSize)
	{
		if (nullptr == pServerIP)
		{
			return false;
		}

		CSelectModel::initSocketSystem();
		mTcpSocket.createSocket();
		int nResult = mTcpSocket.connectServer(pServerIP, nServerPort);
		if (0 != nResult)
		{
			return false;
		}

		mRecvDataBuffer = new byte[nRecvBufSize];
		if (nullptr == mRecvDataBuffer)
		{
			return false;
		}

		mTcpSocket.setRecvBuff(mRecvDataBuffer);
		mTcpSocket.setRecvBuffSize(0);
		mTcpSocket.setMaxRecvBuffSize(nRecvBufSize);
		mSelectModel.addNewSocket(&mTcpSocket, 0);
		if (nullptr != mConnectHandle)
		{
			mConnectHandle();
		}
		return true;
	}

	void CSelectClient::clear()
	{
		if (nullptr != mRecvDataBuffer)
		{
			delete[]mRecvDataBuffer;
		}
	}

	void CSelectClient::receiveMessage()
	{
		if (INVALID_SOCKET == mTcpSocket.getSocketFd())
		{
			return;
		}
		mSelectModel.selectAllFd();
		fd_set& rReadSet = mSelectModel.getReadSet();

		if (!FD_ISSET(mTcpSocket.getSocketFd(), &rReadSet))
		{
			return;
		}

		int nResult = mTcpSocket.recvData(mTcpSocket.getRecvBuffPoint(), mTcpSocket.getRecvBuffCapacity());
		if (nResult <= 0)
		{
			// 客户端已经退出
			clearSocket();
			return;
		}
		else
		{
			mTcpSocket.setRecvBuffSize(mTcpSocket.getRecvBuffSize() + nResult);
			mReceiveMsgHandle(&mTcpSocket);
		}
	}

	int CSelectClient::sendMessage(const byte* pBuff, int nBuffSize)
	{
		if (INVALID_SOCKET == mTcpSocket.getSocketFd())
		{
			return -1;
		}

		int nResult = mTcpSocket.sendData(pBuff, nBuffSize);
		if (nResult != nBuffSize)
		{
			clearSocket();
		}

		return nResult;
	}

	/// 清除socket
	void CSelectClient::clearSocket()
	{
		if (nullptr != mDisconnectHandle)
		{
			mDisconnectHandle();
		}
		mTcpSocket.closeSocket();
		mSelectModel.removeSocket(mTcpSocket.getSocketFd());
	}
}