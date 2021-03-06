#include "platjob.h"
#include "redis.h"
#include "internalmsgpool.h"
#include "locallogjob.h"
#include "gameserver.h"
#include "jobmanager.h"
#include "timemanager.h"
void sendPlatWebRequest(const char* pURL, const char* pData, EmHttpType eHttpType, bool bNeedBack)
{
	CIMPlatWebRequest* pPlatWebRequest = new CIMPlatWebRequest;
	if (NULL == pPlatWebRequest)
	{
		return;
	}

	snprintf(pPlatWebRequest->mURL, MAX_URL_LENGTH - 1, "%s%s", "http://106.13.146.178/", pURL);
	if (NULL == pData)
	{
		strncpy(pPlatWebRequest->mPostData, "", sizeof(pPlatWebRequest->mPostData) - 1);
	}
	else
	{
		if (eHttpType & emHttpTypeFile)
		{
			snprintf(pPlatWebRequest->mPostData, WEB_POST_DATA_LEN - 1, "%s%s", "platfile\\", pData);
		}
		else
		{
			strncpy(pPlatWebRequest->mPostData, pData, sizeof(pPlatWebRequest->mPostData) - 1);
		}
	}
	pPlatWebRequest->mHttpType = eHttpType;
	pPlatWebRequest->mNeedCallBack = bNeedBack;

	CJobManager::Inst()->pushTaskByType(emJobTaskType_Plat, pPlatWebRequest);
}

bool CPlatJob::initAll(const char* pRedisIP, int nRedisPort, int nSocketNum, int nListenPort)
{
	mURLHandle = new CURLHandle;
	if (NULL == mURLHandle)
	{
		return false;
	}
	mURLHandle->setCompleteCallBack(onURLCompleteCallBack);

	bool bResult = mURLHandle->initCURL(100);
	if (!bResult)
	{
		return bResult;
	}

	mAsyncRedis = new CAsyncRedis;
	if (NULL == mAsyncRedis)
	{
		return false;
	}
	bResult = mAsyncRedis->connectRedisServer(pRedisIP, nRedisPort, RedisConnectHandle, RedisDisconnectHandle, CommandCallBack);
	//if (!bResult)
	//{
	//	LOG_ERROR("%s", mAsyncRedis->getErrStr());
	//	return false;
	//}
	
	bResult = initSocket(nSocketNum, nListenPort);
	if (!bResult)
	{
		LOG_ERROR("init socket error!");
		return bResult;
	}
	return true;
}

void CPlatJob::clear()
{
	if (NULL != mURLHandle)
	{
		delete mURLHandle;
		mURLHandle = NULL;
	}

	if (NULL != mAsyncRedis)
	{
		delete mAsyncRedis;
		mAsyncRedis = NULL;
	}

	if (NULL != mSelectModel)
	{
		delete mSelectModel;
		mSelectModel = NULL;
	}

	for (int i = 0; i < mSocketNum; ++ i)
	{
		if (NULL != mTcpSocket[i].getRecvBuff())
		{
			delete mTcpSocket[i].getRecvBuff();
			mTcpSocket[i].setRecvBuff(NULL);
		}
	}
	mSocketNum = 0;
	if (NULL != mTcpSocket)
	{
		delete []mTcpSocket;
		mTcpSocket = NULL;
	}
}

void CPlatJob::doing(int uParam)
{
	mAsyncRedis->doEvent();
	mURLHandle->doing();
	receiveMessage();

	int nCount = 0;
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}

		switch (pIMMsg->getMsgID())
		{
			case IM_REQUEST_PLAT_LOG:
			{
				//onPlatLogRequest(pIMMsg);
				break;
			}
			case IM_REQUEST_PLAT_WEB:
			{
				onPlatWebRequest(pIMMsg);
				break;
			}
			default:
				break;
		}

		++nCount;
		if (nCount > 1000)
		{
			break;
		}
	}

	//// 如果scene job已经退出完成了，表示需要保存的数据基本都完成了
	//if (CSceneJob::Inst()->getExited())
	//{
	//	CInternalMsg* pIMMsg = mTaskManager.popTask();
	//	// 如果没有任务了
	//	if (NULL == pIMMsg)
	//	{
	//		setExited(true);
	//	}
	//}
}

bool CPlatJob::initSocket(int nSocketNum, int nListenPort)
{
	CSelectModel::initSocketSystem();

	mTcpSocket = new CTcpSocket[nSocketNum];
	if (NULL == mTcpSocket)
	{
		LOG_ERROR("new CTcpSocket failure!");
		return false;
	}

	mSelectModel = new CSelectModel(mTcpSocket, nSocketNum);
	if (NULL == mSelectModel)
	{
		LOG_ERROR("new select modle failure!");
		return false;
	}

	time_t tNowTime = CTimeManager::Inst()->getCurrTime();

	int nSocketIndex = -1;
	CTcpSocket* pListenSocket = mSelectModel->createListenSocket(NULL, nListenPort, 5, nSocketIndex);
	if (NULL == pListenSocket)
	{
		LOG_ERROR("create listen socket error!");
		return false;
	}

	pListenSocket->setNonBlock(true);
	pListenSocket->setSendBuffSizeOption(SERVER_SOCKET_CACHE_SIZE);
	pListenSocket->setRecvBuffSizeOption(SERVER_SOCKET_CACHE_SIZE);
	mSocketNum = nSocketNum;
	return true;
}

/// 接收消息
void CPlatJob::receiveMessage()
{
	int nMaxSocketIndex = mSelectModel->getMaxSocketIndex();
	CTcpSocket* pAllSocket = mSelectModel->getAllSocket();
	fd_set& rReadSet = mSelectModel->getReadSet();
	for (int i = 0; i <= nMaxSocketIndex; ++i)
	{
		int nFd = pAllSocket[i].getSocketFd();
		if (INVALID_SOCKET == nFd)
		{
			continue;
		}
		if (FD_ISSET(nFd, &rReadSet))
		{
			if (pAllSocket[i].GetListen())
			{
				int nSocketIndex = -1;
				CTcpSocket* pNewSocket = mSelectModel->getFreeSocket(nSocketIndex);
				pAllSocket[i].acceptConnection(pNewSocket);
				if (NULL == pNewSocket)
				{
					// 出错
					continue;
				}
				if (NULL == pNewSocket->getRecvBuff())
				{
					byte* pNewSocketBuff = new byte[SERVER_SOCKET_BUFF_SIZE];
					if (NULL == pNewSocketBuff)
					{
						clearSocket(nSocketIndex, pNewSocket);
						continue;
					}
					else
					{
						pNewSocket->setRecvBuff(pNewSocketBuff);
					}
				}

				pNewSocket->setMaxRecvBuffSize(SERVER_SOCKET_BUFF_SIZE);
				pNewSocket->setRecvBuffSize(0);
				mSelectModel->addNewSocket(pNewSocket, nSocketIndex);

				printf("IP: %s connect success\n", pNewSocket->getIP());
			}
			else
			{
				int nResult = pAllSocket[i].recvData(pAllSocket[i].getRecvBuffPoint(), pAllSocket[i].getRecvBuffCapacity());
				if (nResult <= 0)
				{
					CTcpSocket* pSocket = mSelectModel->getSocket(i);
					if (NULL != pSocket)
					{
						// 客户端已经退出
						clearSocket(i, pSocket);
					}
					break;
				}
				else
				{
					pAllSocket[i].setRecvBuffSize(pAllSocket[i].getRecvBuffSize() + nResult);
					onReceiveMessage(&(pAllSocket[i]));
				}
			}
		}
	}
}

/// 清除socket
void CPlatJob::clearSocket(int nTcpIndex, CTcpSocket* pSocket)
{
	if (NULL == pSocket)
	{
		return;
	}
	mSelectModel->removeSocket(pSocket->getSocketFd());
	pSocket->closeSocket();
}

/// 收到消息
void CPlatJob::onReceiveMessage(CTcpSocket* pSocket)
{
	if (NULL == pSocket)
	{
		return;
	}

	int nBuffSize = pSocket->getRecvBuffSize();
	byte* pBuffer = pSocket->getRecvBuff();

	int nTotalSize = 0;
	time_t tNowTime = CTimeManager::Inst()->getCurrTime();
	while (true)
	{
		if (nBuffSize < 4)
		{
			break;
		}

		short nMessageLen = *(short*)pBuffer;
		if (nMessageLen < 4 || nMessageLen > SERVER_SOCKET_BUFF_SIZE)
		{
			// 出错
			break;
		}

		// 消息包还没有接受完全
		if (nBuffSize < nMessageLen)
		{
			break;
		}

		pBuffer += nMessageLen;
		nBuffSize -= nMessageLen;
	}

	int nSendSize = pSocket->getRecvBuffSize() - nBuffSize;
	if (nSendSize > 0)
	{
		pSocket->setRecvBuffSize(nBuffSize);
		pSocket->resetRecvBuffPoint(nSendSize);
	}
}

/// 处理平台日志
void CPlatJob::onPlatLogRequest(CInternalMsg* pIMMsg)
{
	if (NULL == pIMMsg)
	{
		return;
	}

	CIMPlatLogRequest* pRequest = (CIMPlatLogRequest*)pIMMsg;
	mAsyncRedis->command(NULL, "LPUSH %s %s", "log_list", pRequest->mContent);	
}

/// 处理Web请求
void CPlatJob::onPlatWebRequest(CInternalMsg* pIMMsg)
{
	if (NULL == pIMMsg)
	{
		return;
	}
	CIMPlatWebRequest* pRequest = (CIMPlatWebRequest*)pIMMsg;
	mURLHandle->sendRequest(pRequest->mURL, pRequest->mPostData, (EmHttpType)(pRequest->mHttpType), pRequest->mNeedCallBack);
}

void CPlatJob::RedisConnectHandle(const struct redisAsyncContext*, int status)
{
	if (NULL == CPlatJob::Inst()->mAsyncRedis)
	{
		return;
	}

	CPlatJob::Inst()->mAsyncRedis->setConnected(true);
}

void CPlatJob::RedisDisconnectHandle(const struct redisAsyncContext*, int status)
{
	if (NULL == CPlatJob::Inst()->mAsyncRedis)
	{
		return;
	}

	CPlatJob::Inst()->mAsyncRedis->setConnected(false);
}

void CPlatJob::CommandCallBack(redisAsyncContext*, void *reply, void *privdata)
{

}

/// Web处理的回调函数
void CPlatJob::onURLCompleteCallBack(CURLSession* pURLSession)
{
	//if (NULL == pURLSession)
	//{
	//	return;
	//}
	//CIMPlatWebResponse* pPlatWebResponse = static_cast<CIMPlatWebResponse*>(CInternalMsgPool::Inst()->allocMsg(IM_RESPONSE_PLAT_WEB));
	//if (NULL == pPlatWebResponse)
	//{
	//	return;
	//}
	//pPlatWebResponse->mHttpType = pURLSession->getHttpType();
	//strncpy(pPlatWebResponse->mReturnData, pURLSession->getReturnData(), WEB_POST_DATA_LEN - 1);
	//CJobManager::Inst()->pushTask(emJobTaskType_Scene, pPlatWebResponse);
}
