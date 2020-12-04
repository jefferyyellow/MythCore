#include "clientsocketjob.h"
#include "messagefactory.h"
#include "entityplayer.h"
#include "internalmsgpool.h"
#include "jobmanager.h"

CPlayerMsgQueue::CPlayerMsgQueue()
{

}

CPlayerMsgQueue::~CPlayerMsgQueue()
{

}

void CPlayerMsgQueue::init(int nReceiveSize, int nSendSize)
{
	byte* pReceiveBuffer = new byte[nReceiveSize];
	mReceiveStream.Initialize(pReceiveBuffer, nReceiveSize);
	byte* pSendBuffer = new byte[nSendSize];
	mSendStream.Initialize(pSendBuffer, nSendSize);
}


void CClientSocketJob::init()
{
	mpSocketBuff = NULL;
}

void CClientSocketJob::clear()
{
	if (NULL != mpSocketBuff)
	{
		delete []mpSocketBuff;
		mpSocketBuff = NULL;
	}

	clearSocket();
}

bool CClientSocketJob::initSocket()
{
	mpSocketBuff = new byte[MAX_GATE_BUFF_SIZE];
	if (NULL == mpSocketBuff)
	{
		return false;
	}
	mTcpSocket.setRecvBuff(mpSocketBuff);
	mTcpSocket.setMaxRecvBuffSize(MAX_GATE_BUFF_SIZE);
	mTcpSocket.setRecvBuffSize(0);
	mTcpSocket.setNonBlock(true);
	mTcpSocket.setSendBuffSizeOption(SOCKET_CACHE_SIZE);
	mTcpSocket.setRecvBuffSizeOption(SOCKET_CACHE_SIZE);
	mSelectModel.addNewSocket(&mTcpSocket, 0);
	return true;
}

void CClientSocketJob::doing(int uParam)
{
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
		onReceivePlayerMsg();
	}
}


/// 清除socket
void CClientSocketJob::clearSocket()
{
	mTcpSocket.closeSocket();
	mSelectModel.removeSocket(mTcpSocket.getSocketFd());
}

void CClientSocketJob::onReceivePlayerMsg()
{
	int nBuffSize = mTcpSocket.getRecvBuffSize();
	byte* pBuffer = mTcpSocket.getRecvBuff();
	for (int i = 0; i < 10000; ++i)
	{
		if (nBuffSize < (int)(4 + sizeof(CExchangeHead)))
		{
			break;
		}

		CExchangeHead* pExchangeHead = (CExchangeHead*)pBuffer;

		pBuffer += sizeof(CExchangeHead);
		nBuffSize -= sizeof(CExchangeHead);

		short nLength = *(short*)pBuffer;
		if (nLength > nBuffSize)
		{
			break;
		}

		CByteStream* pReceiveStream = getReceiveMsgQueue(pExchangeHead->mSocketIndex);
		if (NULL == pReceiveStream)
		{
			// 新的连接
			CPlayerMsgQueue* pMsgQueue = addPlayerMsgQueue(pExchangeHead->mSocketIndex);
			if (NULL != pMsgQueue)
			{
				pReceiveStream = &(pMsgQueue->mReceiveStream);
				
				CIMPlayerLoginRequest* pRequest = new CIMPlayerLoginRequest;
				if (NULL != pRequest)
				{
					pRequest->setMsgID(IM_REQUEST_PLAYER_LOGIN);
					pRequest->mSocketIndex = pExchangeHead->mSocketIndex;
					pRequest->mJobID = getJobID();
					pRequest->mMsgQueue = pMsgQueue;
					CJobManager::Inst()->pushTaskByType(emJobTaskType_Login, pRequest);
				}
			}
		}
		if (NULL != pReceiveStream)
		{
			pReceiveStream->PushPacket(pBuffer, nLength);
		}
		
		//if (NULL != pMessage)
		//{
		//	printf("Recive MessageID: %d\n", nMessageID);
		//	pMessage->ParseFromArray(pBuffer, nMessageLen);
		//	int nModule = nMessageID & MESSAGE_MODULE_MASK;
		//	if (nModule == MESSAGE_MODULE_LOGIN)
		//	{
		//		// 如果已经在场景中了，发登录的消息没有意义
		//		PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(pExchangeHead->mSocketIndex);
		//		if (it != mPlayerSocketList.end())
		//		{
		//			return;
		//		}
		//		CLoginModule::Inst()->onClientMessage(*pExchangeHead, nMessageID, pMessage);
		//	}
		//	else
		//	{
		//		// 还没有登录玩家，发其他的消息没有意义
		//		PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(pExchangeHead->mSocketIndex);
		//		if (it == mPlayerSocketList.end())
		//		{
		//			return;
		//		}

		//		CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
		//		if (NULL == pPlayer)
		//		{
		//			return;
		//		}

		//		// 只有在游戏状态下才接受正常的消息，避免数据的不一致
		//		if (emPlayerStatus_Gameing != pPlayer->getPlayerStauts())
		//		{
		//			return;
		//		}
		//		const ::google::protobuf::Descriptor* pDescriptor = pMessage->GetDescriptor();
		//		LOG_DEBUG("default", "---- Receive from client(Obj Id:%d|Role:%d|Name:%s) Msg[ %s ][id: 0x%04x/%d] ---",
		//			pPlayer->getObjID(), pPlayer->getRoleID(), pPlayer->getName(),
		//			pDescriptor->name().c_str(), nMessageID, nMessageID);
		//		LOG_DEBUG("default", "[%s]", pMessage->ShortDebugString().c_str());
		//		//dispatchClientMessage(*pPlayer, nMessageID, pMessage);
		//	}
		//}
		//else
		//{
		//	// 还没有登录玩家，发其他的消息没有意义
		//	PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(pExchangeHead->mSocketIndex);
		//	if (it == mPlayerSocketList.end())
		//	{
		//		return;
		//	}
		//	/// 玩家没有登录或者已经下线
		//	CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(it->second));
		//	if (NULL == pPlayer)
		//	{
		//		return;
		//	}

		//	// 只有在游戏状态下才接受正常的消息，避免数据的不一致
		//	if (emPlayerStatus_Gameing != pPlayer->getPlayerStauts())
		//	{
		//		return;
		//	}
		//	pBuffer[nMessageLen] = '\0';
		//	//dispatchClientLuaMsg(pPlayer, nMessageID, pTemp);
		//}

		pBuffer += nLength;
		nBuffSize -= nLength;
	}

	int nSendSize = mTcpSocket.getRecvBuffSize() - nBuffSize;
	if (nSendSize > 0)
	{
		mTcpSocket.resetRecvBuffPoint(nSendSize);
		mTcpSocket.setRecvBuffSize(nBuffSize);
	}
}

CPlayerMsgQueue* CClientSocketJob::getPlayerMsgQueue(int nSocketIndex)
{
#ifdef MYTH_OS_UNIX
	if (nSocketIndex >= mPlayerSocketList.size())
	{
		return NULL;
	}
	return mPlayerSocketList[nSocketIndex];
#else
	PLAYER_MSG_QUEUE_LIST::iterator it = mPlayerSocketList.find(nSocketIndex);
	if (it == mPlayerSocketList.end())
	{
		return NULL;
	}
	return it->second;
#endif
}

/// 通过socket索引得到接收消息队列
CByteStream* CClientSocketJob::getReceiveMsgQueue(int nSocketIndex)
{
#ifdef MYTH_OS_UNIX
	if (nSocketIndex >= mPlayerSocketList.size())
	{
		return NULL;
	}
	if (NULL == mPlayerSocketList[nSocketIndex])
	{
		return NULL;
	}
	return &(mPlayerSocketList[nSocketIndex]->mReceiveStream);
#else
	PLAYER_MSG_QUEUE_LIST::iterator it = mPlayerSocketList.find(nSocketIndex);
	if (it == mPlayerSocketList.end())
	{
		return NULL;
	}
	if (NULL == it->second)
	{
		return NULL;
	}
	return &(it->second->mReceiveStream);
#endif
}

CPlayerMsgQueue* CClientSocketJob::addPlayerMsgQueue(int nSocketIndex)
{
#ifdef MYTH_OS_UNIX
	if (nSocketIndex >= mPlayerSocketList.size())
	{
		mPlayerSocketList.resize(nSocketIndex + 1, NULL);
	}
	if (NULL != mPlayerSocketList[nSocketIndex])
	{
		return mPlayerSocketList[nSocketIndex];
	}

	CPlayerMsgQueue* pQueue = new CPlayerMsgQueue;
	if (NULL == pQueue)
	{
		return;
	}
	pQueue->init(PLAYER_SOCKET_BUFF_SIZE, PLAYER_SOCKET_BUFF_SIZE);
	mPlayerSocketList[nSocketIndex] = pQueue;
	return pQueue;
#else
	PLAYER_MSG_QUEUE_LIST::iterator it = mPlayerSocketList.find(nSocketIndex);
	if (it != mPlayerSocketList.end())
	{
		return it->second;
	}
	CPlayerMsgQueue* pQueue = new CPlayerMsgQueue;
	if (NULL == pQueue)
	{
		return NULL;
	}
	pQueue->init(PLAYER_SOCKET_BUFF_SIZE, PLAYER_SOCKET_BUFF_SIZE);

	mPlayerSocketList.insert(PLAYER_MSG_QUEUE_LIST::value_type(nSocketIndex, pQueue));
	return pQueue;
#endif
}
