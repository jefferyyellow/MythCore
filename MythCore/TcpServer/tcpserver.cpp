#include "tcpserver.h"
#include "tinyxml2.h"
#include "timemanager.h"
#include <errno.h>

#	define LOG_INFO(fmt, ... )				CLogManager::Inst()->LogInfoMessageFormat(fmt, ##__VA_ARGS__ )
#	define LOG_WARN(fmt, ... )				CLogManager::Inst()->LogWarnMessageFormat(fmt, ##__VA_ARGS__ )
#	define LOG_ERROR(fmt, ... )				CLogManager::Inst()->LogErrorMessageFormat(fmt, ##__VA_ARGS__ )
#	define LOG_DEBUG(logname, fmt, ... )	CLogManager::Inst()->LogDebugMessageFormat(logname, fmt, ##__VA_ARGS__ )

using namespace tinyxml2;

CTcpServer::CTcpServer()
{

}

CTcpServer::~CTcpServer()
{

}

/// 初始化
bool CTcpServer::init()
{
	CTimeManager* pTimeManager = CTimeManager::createInst();
	if (NULL == pTimeManager)
	{
		return false;
	}
	pTimeManager->setCurrTime(time(NULL));
	mLastTime = pTimeManager->getCurrTime();
	mLastStatisticsTime = mLastTime;
	mLastKeepLiveTime = mLastTime;

	bool bResult = initLog();
	if (!bResult)
	{
		return false;
	}

	loadTcpServerConfig("config/tcpserverconfig.xml");

	bResult = initShareMemory();
	if (!bResult)
	{
		return false;
	}

	bResult = initSocket();
	return bResult;
}

/// 初始日志
bool CTcpServer::initLog()
{
	CLogManager* pLogManger = CLogManager::createInst();
	if (NULL == pLogManger)
	{
		printf("create log manager failure");
		return false;
	}

#ifdef __DEBUG__
	// 默认日志
	mDefaultLog = new CLog();
	if (NULL == mDefaultLog)
	{
		printf("create default log failure");
		return false;
	}

	// 默认的debug日志
	CRollFileDisplayer* pDefaultFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/tcpdefault.log"), 1024000, 10);
	// 为默认的debug日志加文件displayer
	mDefaultLog->AddDisplayer(pDefaultFileDisplayer);
	CLogManager::Inst()->AddDebugLog(mDefaultLog, "default");
#endif

	// 错误日志加文件displayer
	CRollFileDisplayer* pErrorFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/tcperror.log"), 1024000, 10);
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pErrorFileDisplayer);

	// 错误日志加std displayer
	CStdDisplayer* pDisplayer = new CStdDisplayer();
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pDisplayer);

	// 给信息日志加文件displayer
	CRollFileDisplayer* pInfoFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/tcpinfo.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pInfoFileDisplayer);

	// 给警告日志加文件displayer
	CRollFileDisplayer* pWarnFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/tcpwarn.log"), 1024000, 10);
	CLogManager::Inst()->GetWarnLog().AddDisplayer(pWarnFileDisplayer);

	mStatisticsLog = new CLog();
	if (NULL == mStatisticsLog)
	{
		printf("create StatisticsLog log failure");
		return false;
	}

	// 默认的debug日志
	CRollFileDisplayer* pStatisticsFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/tcpstat.log"), 1024000, 10);
	// 为默认的debug日志加文件displayer
	mStatisticsLog->AddDisplayer(pStatisticsFileDisplayer);

	CLogManager::Inst()->setTmNow(CTimeManager::Inst()->getCurrTime());
	return true;
}

/// 初始化管道
bool CTcpServer::initShareMemory()
{
	// 一个CShareMemory管理类
	int nShareMemorySize = sizeof(CShareMemory);
	// 两个CSocketStream管理类
	nShareMemorySize += 2 * sizeof(CSocketStream);
	// 两个真实的内存区域
	nShareMemorySize += 2 * PIPE_SIZE;

	bool bCreate = true;
	byte* pSharePoint = CShareMemory::createShareMemory(37345234, nShareMemorySize, bCreate);
	if (NULL == pSharePoint)
	{
		return false;
	}
	// 初始化
	mShareMemory = (CShareMemory*)pSharePoint;
	if (bCreate)
	{
		mShareMemory->setShmPoint(pSharePoint);
		mShareMemory->setShmKey(37345234);
		mShareMemory->setShmSize(nShareMemorySize);
	}


	pSharePoint += sizeof(CShareMemory);

	// 初始化Tcp2Server共享内存
	mTcp2ServerMemory = (CSocketStream*)pSharePoint;
	if (bCreate)
	{
		mTcp2ServerMemory->Initialize(pSharePoint + sizeof(CSocketStream), PIPE_SIZE);
	}

	pSharePoint += sizeof(CSocketStream) + PIPE_SIZE;
	// 初始化Server2Tcp共享内存
	mServer2TcpMemory = (CSocketStream*)pSharePoint;
	if (bCreate)
	{
		mServer2TcpMemory->Initialize(pSharePoint + sizeof(CSocketStream), PIPE_SIZE);
	}
	
	return true;
}


#ifdef MYTH_OS_WINDOWS
/// 初始化Socket
bool CTcpServer::initSocket()
{
	CSelectModel::initSocketSystem();

	mTcpSocket = new CTcpSocket[MAX_SOCKET_NUM];
	if (NULL == mTcpSocket)
	{
		return false;
	}

	mSocketInfo = new CSocketInfo[MAX_SOCKET_NUM];
	if (NULL == mSocketInfo)
	{
		return false;
	}

	mSelectModel = new CSelectModel(mTcpSocket, MAX_SOCKET_NUM);
	if (NULL == mSelectModel)
	{
		return false;
	}

	time_t tNowTime = CTimeManager::Inst()->getCurrTime();
	for (int i = 0; i < MAX_LISTEN_PORT_NUM; ++ i)
	{
		if (mTcpConfig.mListenPort[i] == 0)
		{
			break;
		}
		int nSocketIndex = -1;
		CTcpSocket* pListenSocket = mSelectModel->createListenSocket(NULL, mTcpConfig.mListenPort[i], 5, nSocketIndex);
		if (NULL == pListenSocket)
		{
			return false;
		}

		if (nSocketIndex >= 0 && nSocketIndex < MAX_SOCKET_NUM)
		{
			mSocketInfo[nSocketIndex].mCreateTime = 0;
			mSocketInfo[nSocketIndex].mKeepLiveTime = 0;
		}


		pListenSocket->setNonBlock(true);
		pListenSocket->setSendBuffSizeOption(SOCKET_CACHE_SIZE);
		pListenSocket->setRecvBuffSizeOption(SOCKET_CACHE_SIZE);
	}

	return true;
}
#else
/// 初始化Socket
bool CTcpServer::initSocket()
{
	mTcpSocket = new CTcpSocket[MAX_SOCKET_NUM];
	if (NULL == mTcpSocket)
	{
		return false;
	}

	mSocketInfo = new CSocketInfo[MAX_SOCKET_NUM];
	if (NULL == mSocketInfo)
	{
		return false;
	}

	mEpollModel = new CEpollModel(mTcpSocket, MAX_SOCKET_NUM);
	if (NULL == mEpollModel)
	{
		return false;
	}
	mEpollModel->initEpollSocket();

	time_t tNowTime = CTimeManager::Inst()->getCurrTime();
	for (int i = 0; i < MAX_LISTEN_PORT_NUM; ++ i)
	{
		if (mTcpConfig.mListenPort[i] == 0)
		{
			break;
		}
	
		CTcpSocket* pListenSocket = mEpollModel->createListenSocket(NULL, mTcpConfig.mListenPort[i], 5);
		if (NULL == pListenSocket)
		{
			return false;
		}
		int nSocketIndex = pListenSocket->getSocketFd();
		if (nSocketIndex >= 0 && nSocketIndex < MAX_SOCKET_NUM)
		{
			mSocketInfo[nSocketIndex].mCreateTime = tNowTime;
			mSocketInfo[nSocketIndex].mKeepLiveTime = tNowTime;
		}

		pListenSocket->setNonBlock(true);
		pListenSocket->setSendBuffSizeOption(8192);
		pListenSocket->setRecvBuffSizeOption(8192);
	}
}
#endif

/// 运行
void CTcpServer::run()
{
	static int i = 0;
	while (true)
	{
		CTimeManager::Inst()->setCurrTime(time(NULL));
		uint64 tTimeNow = CTimeManager::Inst()->getCurrTime();
		if (tTimeNow != mLastTime)
		{
			CLogManager::Inst()->setTmNow(tTimeNow);
			mLastTime = tTimeNow;
		}
#ifdef MYTH_OS_WINDOWS
		mSelectModel->selectAllFd();
		Sleep(1);
#else
		struct timespec tv;
		tv.tv_sec = 0;
		tv.tv_nsec = 1000;

		nanosleep(&tv, NULL);
#endif
		receiveMessage();
		sendMessage();
		checkTimeOut();
	}
	
}

/// 检查是否超时
void CTcpServer::checkTimeOut()
{
	time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
	if (tTimeNow - mLastStatisticsTime > mTcpConfig.mWriteStatisticsTime)
	{
		writeTcpStatisticsData();
		mLastStatisticsTime = tTimeNow;
	}

	// 检查keep live的时间
	if (tTimeNow - mLastKeepLiveTime > mTcpConfig.mCheckLiveTime)
	{
		checkKeepLiveTimeOut(tTimeNow);
		mLastKeepLiveTime = tTimeNow;
	}
}

/// 检测keep live 是否超时
void CTcpServer::checkKeepLiveTimeOut(time_t tTimeNow)
{
#ifdef MYTH_OS_WINDOWS
	int nMaxSocketIndex = mSelectModel->getMaxSocketIndex();
#else
	int nMaxSocketIndex = mEpollModel->getMaxSocketFd();
#endif
	for (int i = 0; i <= nMaxSocketIndex; ++i)
	{
		// 没有用的socket，直接跳过
		if (mSocketInfo[i].mCreateTime == 0)
		{
			continue;
		}
		// 是否在keep live期间没有任何数据到来
		if (tTimeNow - mSocketInfo[i].mKeepLiveTime > mTcpConfig.mKeepLiveTime)
		{
#ifdef MYTH_OS_WINDOWS
			CTcpSocket* pSocket = mSelectModel->getSocket(i);
#else
			CTcpSocket* pSocket = mEpollModel->getSocket(i);
#endif
			if (NULL == pSocket)
			{
				continue;
			}
			// 直接删除
			clearSocketInfo(i, pSocket);
			sendSocketErrToGameServer(i, emTcpError_KeepLive);
		}
	}
}

#ifdef MYTH_OS_WINDOWS
/// 接收消息
void CTcpServer::receiveMessage()
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
					byte* pNewSocketBuff = new byte[MAX_SOCKET_BUFF_SIZE];
					if (NULL == pNewSocketBuff)
					{
						clearSocketInfo(nSocketIndex, pNewSocket);
						continue;
					}
					else
					{
						pNewSocket->setRecvBuff(pNewSocketBuff);
					}
				}

				pNewSocket->setMaxRecvBuffSize(MAX_SOCKET_BUFF_SIZE);
				pNewSocket->setRecvBuffSize(0);
				mSelectModel->addNewSocket(pNewSocket, nSocketIndex);

				time_t tNowTime = CTimeManager::Inst()->getCurrTime();
				if (nSocketIndex>= 0 && nSocketIndex < MAX_SOCKET_NUM)
				{
					mSocketInfo[nSocketIndex].mCreateTime = tNowTime;
					mSocketInfo[nSocketIndex].mKeepLiveTime = tNowTime;
				}


				// 时间周期内的连接数加一
				++ mServerStatistics.mConnects;
				// 连接总数加一
				++ mServerStatistics.mTotalConnects;

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
						clearSocketInfo(i, pSocket);
						sendSocketErrToGameServer(i, emTcpError_SendData);
					}
					break;
				}
				else
				{
					pAllSocket[i].setRecvBuffSize(pAllSocket[i].getRecvBuffSize() + nResult);
					onReceiveMessage(&(pAllSocket[i]), i);
					printf("receive message %d\n", i);
				}
			}
		}
	}
}
#else
void CTcpServer::receiveMessage()
{
	int nNumFd = mEpollModel->wait();
	if (nNumFd < 0)
	{
		// 出错，记录错误日志
	}

	struct epoll_event* pEvent = mEpollModel->getWaitEvents();
	CTcpSocket* pAllSocket = mEpollModel->getAllSocket();
	int nSocketCapacity = mEpollModel->SocketCapacity();
	int nFd = -1;
	for (int i = 0; i < nNumFd; i++, pEvent++)
	{
		nFd = pEvent->data.fd;
		if (nFd < 0 || nFd >= nSocketCapacity)
		{
			LOG_ERROR("epoll wait event error, Fd: %d", nFd);
			continue;
		}
		CTcpSocket& rTcpSocket = pAllSocket[nFd];
		// error
		if (0 != (EPOLLERR & pEvent->events))
		{
			// 出错
			int nSocketError = rTcpSocket.getSocketErrNo();
			LOG_ERROR("epoll wait event error, Fd: %d, Events: %d, ErrNo: %d Error: %s", nFd, pEvent->events, nSocketError, strerror(nSocketError));
			clearSocketInfo(rTcpSocket.getSocketFd(), &rTcpSocket);
			sendSocketErrToGameServer(rTcpSocket.getSocketFd(), emTcpError_SendData);
			continue;
		}


		// 不可读，直接滚蛋
		if (0 == (EPOLLIN & pEvent->events))
		{
			clearSocketInfo(nFd, &pAllSocket[nFd]);
			sendSocketErrToGameServer(i, emTcpError_ReadData);
			continue;
		}

		// listen socket
		if (rTcpSocket.GetListen())
		{
			CTcpSocket* pNewSocket = mEpollModel->acceptConnection(rTcpSocket);

			if (NULL == pNewSocket)
			{
				// 出错
				continue;
			}
			if (NULL == pNewSocket->getRecvBuff())
			{
				byte* pNewSocketBuff = new byte[MAX_SOCKET_BUFF_SIZE];
				if (NULL == pNewSocketBuff)
				{
					clearSocketInfo(pNewSocket->getSocketFd(), pNewSocket);
					continue;
				}
				else
				{
					pNewSocket->setRecvBuff(pNewSocketBuff);
				}
			}
			pNewSocket->setMaxRecvBuffSize(MAX_SOCKET_BUFF_SIZE);
			pNewSocket->setRecvBuffSize(0);

			time_t tNowTime = CTimeManager::Inst()->getCurrTime();  
			int nFd = pNewSocket->getSocketFd();
			if (nFd>= 0 && nFd < MAX_SOCKET_NUM)
			{
				mSocketInfo[nFd].mCreateTime = tNowTime;
				mSocketInfo[nFd].mKeepLiveTime = tNowTime;
			}

			// 时间周期内的连接数加一
			++ mServerStatistics.mConnects;
			// 连接总数加一
			++ mServerStatistics.mTotalConnects;

			printf("IP: %s connect success\n", pNewSocket->getIP());
		}
		else
		{
			int nResult = rTcpSocket.recvData(rTcpSocket.getRecvBuffPoint(), rTcpSocket.getRecvBuffCapacity());
			if (nResult <= 0)
			{
				// 客户端已经退出
				clearSocketInfo(rTcpSocket.getSocketFd(), &rTcpSocket);
				sendSocketErrToGameServer(rTcpSocket.getSocketFd(), emTcpError_SendData);
				break;
			}
			else
			{
				rTcpSocket.setRecvBuffSize(rTcpSocket.getRecvBuffSize() + nResult);
				onReceiveMessage(&rTcpSocket, nFd);
				printf("receive message\n");
			}
		}
	}
}
#endif


// 服务器收到一个前端消息
void CTcpServer::onReceiveMessage(CTcpSocket* pSocket, int nIndex)
{
	if (NULL == pSocket)
	{
		return;
	}
	if (nIndex <= 0 || nIndex >= MAX_SOCKET_NUM)
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
		if (nMessageLen < 4 || nMessageLen > MAX_SOCKET_BUFF_SIZE)
		{
			// 出错
			break;
		}

		// 消息包还没有接受完全
		if (nBuffSize < nMessageLen)
		{
			break;
		}

		mExchangeHead.mSocketIndex = nIndex;
		mExchangeHead.mSocketError = emTcpError_None;
		mExchangeHead.mSocketTime = mSocketInfo[nIndex].mCreateTime;
		mSocketInfo[nIndex].mKeepLiveTime = tNowTime;

		memcpy(mBuffer, &mExchangeHead, sizeof(mExchangeHead));
		memcpy(mBuffer + sizeof(mExchangeHead), pBuffer, nMessageLen);
		mTcp2ServerMemory->PushPacket((byte*)mBuffer, nMessageLen + sizeof(mExchangeHead));

		pBuffer += nMessageLen;
		nBuffSize -= nMessageLen;

		// 增加收到的消息数目
		++mServerStatistics.mReceiveMessageNum;
		// 增加收到的字节
		mServerStatistics.mReceiveBytes += nMessageLen;
	}

	int nSendSize = pSocket->getRecvBuffSize() - nBuffSize;
	if (nSendSize > 0)
	{
		pSocket->setRecvBuffSize(nBuffSize);
		pSocket->resetRecvBuffPoint(nSendSize);
	}
}

// 发送客户端消息
void CTcpServer::sendMessage()
{
	int nMessageLen = MAX_SOCKET_BUFF_SIZE;
	int nResult = 0;
	for (int i = 0; i < MAX_SEND_PACKAGE_ONCE; ++ i)
	{
		nResult = mServer2TcpMemory->GetHeadPacket((byte*)mBuffer, nMessageLen);
		if (nResult < 0)
		{
			break;
		}

		if (nMessageLen <= 0)
		{
			break;
		}
		byte* pTemp = mBuffer;
		CExchangeHead* pExchangeHead = (CExchangeHead*)mBuffer;

		pTemp += sizeof(CExchangeHead);
		nMessageLen -= sizeof(CExchangeHead);

		int nTcpIndex = pExchangeHead->mSocketIndex;
		if (nTcpIndex <= 0 || nTcpIndex >= MAX_SOCKET_NUM)
		{
			continue;
		}

		if (mSocketInfo[nTcpIndex].mCreateTime != pExchangeHead->mSocketTime)
		{
			continue;
		}

		printf("CTcpServer::sendMessage nMessageLen: %d\n", nMessageLen);

#ifdef MYTH_OS_WINDOWS
		CTcpSocket* pSocket = mSelectModel->getSocket(nTcpIndex);
		if (NULL == pSocket)
		{
			continue;
		}
		if (INVALID_SOCKET == pSocket->getSocketFd())
		{
			continue;
		}

		// 游戏服务器已经关闭socket，所以不需要通知游戏服务器了
		if (pExchangeHead->mSocketError == emTcpError_OffLineClose)
		{
			clearSocketInfo(nTcpIndex, pSocket);
			continue;
		}

		if (nMessageLen <= 0)
		{
			continue;
		}
		short nLength = *(short*)pTemp;
		if (nLength != nMessageLen)
		{
			continue;
		}

		int nResult = pSocket->sendData(pTemp, nMessageLen);
		// int nResult = mSelectModel->processWrite(nTcpIndex, pTemp, nMessageLen);
		if (nResult != nMessageLen)
		{
			sendSocketErrToGameServer(nTcpIndex, emTcpError_SendData);
			clearSocketInfo(nTcpIndex, pSocket);
			continue;
		}
		// 增加发送消息数目
		++ mServerStatistics.mSendMessageNum;
		// 增加发送消息的字节数
		mServerStatistics.mSendBytes += nMessageLen;
#else

		CTcpSocket* pSocket = mEpollModel->getSocket(nTcpIndex);
		if (NULL == pSocket)
		{
			continue;
		}
		// 游戏服务器已经关闭socket，所以不需要通知游戏服务器了
		if (pExchangeHead->mSocketError == emTcpError_OffLineClose)
		{
			clearSocketInfo(nTcpIndex, pSocket);
			continue;
		}

		short nLength = *(short*)pTemp;
		if (nLength != nMessageLen)
		{
			continue;
		}

		int nResult = pSocket->sendData(pTemp, nMessageLen);
		// int nResult = mEpollModel->processWrite(nTcpIndex, pTemp, nMessageLen);
		if (nResult != nMessageLen)
		{
			sendSocketErrToGameServer(nTcpIndex, emTcpError_SendData);
			clearSocketInfo(nTcpIndex, pSocket);
			continue;
		}

		// 增加发送消息数目
		++mServerStatistics.mSendMessageNum;
		// 增加发送消息的字节数
		mServerStatistics.mSendBytes += nMessageLen;
#endif

	}
}

// 通知游戏服务器删除一个socket
void CTcpServer::sendSocketErrToGameServer(int nTcpIndex, short nSocketError)
{
	if (nTcpIndex <= 0 || nTcpIndex >= MAX_SOCKET_NUM)
	{
		return;
	}

	mExchangeHead.mSocketIndex = nTcpIndex;
	mExchangeHead.mSocketError = nSocketError;
	mExchangeHead.mSocketTime = mSocketInfo[nTcpIndex].mCreateTime;

	memcpy(mBuffer, &mExchangeHead, sizeof(mExchangeHead));
	mTcp2ServerMemory->PushPacket((byte*)mBuffer, sizeof(mExchangeHead));
}

// 清除socket info
void CTcpServer::clearSocketInfo(int nTcpIndex, CTcpSocket* pSocket)
{
	if (nTcpIndex >= 0 && nTcpIndex < MAX_SOCKET_NUM)
	{
		mSocketInfo[nTcpIndex].mCreateTime = 0;
	}
	
#ifdef MYTH_OS_WINDOWS
	mSelectModel->removeSocket(pSocket->getSocketFd());
#else
	mEpollModel->delSocket(pSocket->getSocketFd());
#endif
	pSocket->closeSocket();
	// 总连接数减一
	-- mServerStatistics.mTotalConnects;
	printf("client disconnect, nTcpIndex: %d", nTcpIndex);
}

// 加载TCP服务器配置
void CTcpServer::loadTcpServerConfig(const char* pConfigPath)
{
	if (NULL == pConfigPath)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pConfigPath))
	{
		// 出错，无法加载xml文件
		return;
	}
	
	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// 出错，没有root节点
		return;
	}

	// 侦听端口
	XMLElement* pListPortElement = pRoot->FirstChildElement("ListPort");
	if (NULL != pListPortElement)
	{
		int nCount = 0;
		XMLElement* pPortElement = pListPortElement->FirstChildElement("Port");
		for (; NULL != pPortElement; 
			pPortElement = pListPortElement->NextSiblingElement("Port"))
		{
			mTcpConfig.mListenPort[nCount] = pPortElement->IntAttribute("value");
			++ nCount;
			if (nCount >= MAX_LISTEN_PORT_NUM)
			{
				break;
			}
		}
	}

	XMLElement* pRunParamElement = pRoot->FirstChildElement("RunParam");
	if (NULL != pRunParamElement)
	{
		XMLElement* pKeepLiveTimeElement = pRunParamElement->FirstChildElement("KeepLiveTime");
		if (NULL != pKeepLiveTimeElement)
		{
			mTcpConfig.mKeepLiveTime = pKeepLiveTimeElement->IntAttribute("value");
		}

		XMLElement* pCheckLiveTimeElement = pRunParamElement->FirstChildElement("CheckLiveTime");
		if (NULL != pCheckLiveTimeElement)
		{
			mTcpConfig.mCheckLiveTime = pCheckLiveTimeElement->IntAttribute("value");
		}

		XMLElement* pWriteStatisticsTimeElement = pRunParamElement->FirstChildElement("WriteStatisticsTime");
		if (NULL != pWriteStatisticsTimeElement)
		{
			mTcpConfig.mWriteStatisticsTime = pWriteStatisticsTimeElement->IntAttribute("value");
		}
	}
}

/// 写统计信息
void CTcpServer::writeTcpStatisticsData()
{ 

	int nServer2TcpLeftSpace = mServer2TcpMemory->GetLeftSpace();
	int nTcp2ServerLeftSpace = mTcp2ServerMemory->GetLeftSpace();

	char tBuff[256] = {0};
	snprintf(tBuff, sizeof(tBuff) - 1, "S2TLeft: %d --- T2S: %d, Connects: %d, TotalConnects: %d, ReceiveBytes: %d, ReceiveMessageNum: %d, SendBytes: %d, SendMessageNum:%d",
		nServer2TcpLeftSpace, nTcp2ServerLeftSpace, mServerStatistics.mConnects, mServerStatistics.mTotalConnects, mServerStatistics.mReceiveBytes, 
		mServerStatistics.mReceiveMessageNum, mServerStatistics.mSendBytes, mServerStatistics.mSendMessageNum);


	char tLogBuff[256] = { 0 };
	CLogManager::Inst()->FormatLogMessage(tLogBuff, sizeof(tLogBuff) - 1, "INFO", tBuff);
	mStatisticsLog->DisplayLog(tLogBuff);


	mServerStatistics.mConnects = 0;
	mServerStatistics.mReceiveBytes = 0;
	mServerStatistics.mReceiveMessageNum = 0;
	mServerStatistics.mSendBytes = 0;
	mServerStatistics.mSendMessageNum = 0;
}

/// 清除日志
void CTcpServer::clearLog(CLog* pLog)
{
	if (NULL == pLog)
	{
		return;
	}

	for (int i = 0; i < pLog->GetDisplayerSize(); ++i)
	{
		CLogDisplayer* pDisplay = pLog->GetDisplayer(i);
		if (NULL != pDisplay)
		{
			delete pDisplay;
		}
	}
}

/// 开始为退出做准备
void CTcpServer::clear()
{
	for (int i = 0; i < MAX_SOCKET_NUM; ++ i)
	{
		byte* pBuff = mTcpSocket[i].getRecvBuff();
		if (NULL != pBuff)
		{
			delete[]pBuff;
		}
	}

	delete[]mTcpSocket;
	delete[]mSocketInfo;
#ifdef MYTH_OS_WINDOWS
	delete mSelectModel;
#else
	delete mEpollModel;
#endif

#ifdef __DEBUG__
	clearLog(mDefaultLog);
	delete mDefaultLog;
#endif // __DEBUG__
	clearLog(&CLogManager::Inst()->GetErrorLog());
	clearLog(&CLogManager::Inst()->GetInfoLog());
	clearLog(&CLogManager::Inst()->GetWarnLog());
	clearLog(mStatisticsLog);
	delete mStatisticsLog;

	CLogManager::destroyInst();
	CTimeManager::destroyInst();
}

/// 退出
void CTcpServer::exit()
{

}