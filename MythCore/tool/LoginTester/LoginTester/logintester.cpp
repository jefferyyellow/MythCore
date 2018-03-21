#include "logintester.h"
#include "logmanager.h"
#include "logdisplayer.h"
#include "loginmessage.hxx.pb.h"
#include "player.h"
CLoginTester::CLoginTester()
	:mSelectModel(mTcpSocket, MAX_SOCKET_NUM), mResetTimer(300)
{
	mDefaultLog = NULL;
	mServerIP[0] = '\0';
	mServerPort = 0;
	mBuffer[0] = { 0 };
}

CLoginTester::~CLoginTester()
{

}

/// 初始化
bool CLoginTester::init()
{
	srand(time(NULL));
	CSelectModel::initSocketSystem();

	if (!initLog())
	{
		return false;
	}
	mLastTickCount = getTickCount();
	mAccountNameCount = 0;
	return true;
}

/// 初始日志
bool CLoginTester::initLog()
{
	CLogManager* pLogManger = CLogManager::CreateInst();
	if (NULL == pLogManger)
	{
		printf("create log manager failure\n");
		return false;
	}

#ifdef __DEBUG__
	// 默认日志
	mDefaultLog = new CLog();
	if (NULL == mDefaultLog)
	{
		printf("create default log failure\n");
		return false;
	}
#endif

	// 默认的debug日志
	CRollFileDisplayer* pDefaultFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/clientdefault.log"), 1024000, 10);
	// 为默认的debug日志加文件displayer
	mDefaultLog->AddDisplayer(pDefaultFileDisplayer);
	CLogManager::Inst()->AddDebugLog(mDefaultLog, "default");

	// 错误日志加文件displayer
	CRollFileDisplayer* pErrorFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/clienterror.log"), 1024000, 10);
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pErrorFileDisplayer);

	// 错误日志加std displayer
	CStdDisplayer* pDisplayer = new CStdDisplayer();
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pDisplayer);

	// 给信息日志加文件displayer
	CRollFileDisplayer* pInfoFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/clientinfo.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pInfoFileDisplayer);

	// 给警告日志加文件displayer
	CRollFileDisplayer* pWarnFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/clientwarn.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pWarnFileDisplayer);
	return true;
}

/// 运行
void CLoginTester::run()
{
	while (true)
	{
		mSelectModel.selectAllFd();
		processServerMessage();
		Sleep(30);

		uint64 nTickCount = getTickCount();
		onTime((int)(nTickCount - mLastTickCount));
		mLastTickCount = nTickCount;
	}
}



/// 处理前端消息
void CLoginTester::processServerMessage()
{
	int nMaxSocketIndex = mSelectModel.getMaxSocketIndex();
	CTcpSocket* pAllSocket = mSelectModel.getAllSocket();
	fd_set& rReadSet = mSelectModel.getReadSet();
	for (int i = 0; i <= nMaxSocketIndex; ++i)
	{
		int nFd = pAllSocket[i].getSocketFd();
		if (INVALID_SOCKET == nFd)
		{
			continue;
		}
		if (FD_ISSET(nFd, &rReadSet))
		{
			int nResult = pAllSocket[i].recvData(pAllSocket[i].getRecvBuffPoint(), pAllSocket[i].getRecvBuffCapacity());
			if (nResult <= 0)
			{
				// 客户端已经退出
				int nRemoveFd = pAllSocket[i].getSocketFd();
				pAllSocket[i].closeSocket();
				mSelectModel.removeSocket(nRemoveFd);
				printf("disconnect with server!!!\n");
				PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(i);
				if (it != mPlayerSocketList.end())
				{
					mPlayerSocketList.erase(it);
				}
				break;
			}
			else
			{
				pAllSocket[i].setRecvBuffSize(pAllSocket[i].getRecvBuffSize() + nResult);

				CPlayer* pPlayer = NULL;
				PLAYER_SOCKET_LIST::iterator it = mPlayerSocketList.find(i);
				if (it != mPlayerSocketList.end())
				{
					pPlayer = it->second;
				}
				onServerMessage(&pAllSocket[i], pPlayer);
			}
		}
	}
}

/// 当收到服务器消息
void CLoginTester::onServerMessage(CTcpSocket* pTcpSocket, CPlayer* pPlayer)
{
	if (NULL == pTcpSocket)
	{
		return;
	}

	byte* pTemp = pTcpSocket->getRecvBuff();
	if (pTcpSocket->getRecvBuffSize() < sizeof(unsigned short) * 2)
	{
		return;
	}

	short nLength = *(short*)pTemp;
	pTemp += sizeof(short);

	short nMessageID = *(short*)pTemp;
	pTemp += sizeof(short);
	printf("receive message MessageID : %d\n", nMessageID);
	Message* pMessage = createMessage(nMessageID);
	if (NULL != pMessage)
	{
		pMessage->ParseFromArray(pTemp, nLength - sizeof(unsigned short) * 2);
		pTcpSocket->resetRecvBuffPoint(nLength);

		pPlayer->onServerMessage(nMessageID, pMessage);
	}
}

/// 创建消息
Message* CLoginTester::createMessage(unsigned short nMessageID)
{
	Message* pMessage = NULL;
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_LOGIN:
		{
			pMessage = new CLoginRequest;
			break;
		}
		case ID_S2C_RESPONSE_LOGIN:
		{
			pMessage = new CLoginResponse;
			break;
		}
		case ID_C2S_REQUEST_CREATE_ROLE:
		{
			pMessage = new CCreateRoleRequest;
			break;
		}
		case ID_S2C_RESPONSE_CREATE_ROLE:
		{
			pMessage = new CCreateRoleResponse;
			break;
		}
		case ID_C2S_REQUEST_ENTER_SCENE:
		{
			pMessage = new CEnterSceneRequest;
			break;
		}
		case ID_S2C_RESPONSE_ENTER_SCENE:
		{
			pMessage = new CEnterSceneResponse;
			break;
		}
		default:
			break;
	}

	return pMessage;
}

/// 发送消息给服务器
void CLoginTester::sendMessage(int nTcpIndex, unsigned short uMessageID, Message* pMessage)
{
	CTcpSocket* pTcpSocket = mSelectModel.getSocket(nTcpIndex);
	if (NULL == pTcpSocket)
	{
		return;
	}

	printf("sendMessage ID: %d\n", uMessageID);
	// 一个 unsigned short是消息ID，另一个是消息长度
	unsigned short nMessageLen = pMessage->ByteSize() + sizeof(unsigned short) * 2;
	byte* pTemp = mBuffer;
	memcpy(pTemp, &nMessageLen, sizeof(nMessageLen));
	pTemp += sizeof(nMessageLen);
	memcpy(pTemp, &uMessageID, sizeof(uMessageID));
	pTemp += sizeof(uMessageID);

	pMessage->SerializeToArray(pTemp, sizeof(mBuffer) - (sizeof(unsigned short) * 2));
	pTcpSocket->sendData(mBuffer, nMessageLen);
}

void CLoginTester::newPlayer(const char* pAccountName, int nChannelID, int nServerID)
{
	CPlayer* pNewPlayer = new CPlayer;
	if (NULL == pNewPlayer)
	{
		printf("new player failure!\n");
		return;
	}
	byte* pReceiveBuff = new byte[MAX_PLAYER_SOCKET_BUFF];
	if (NULL == pReceiveBuff)
	{
		printf("new receive buff failure!\n");
		return;
	}
	int nSocketIndex = -1;
	CTcpSocket* pNewSocket = mSelectModel.getFreeSocket(nSocketIndex);
	if (NULL == pNewSocket)
	{
		printf("assign socket failure!\n");
		return;
	}
	pNewSocket->createSocket();
	pNewSocket->setRecvBuff(pReceiveBuff);
	pNewSocket->setRecvBuffSize(0);
	pNewSocket->setMaxRecvBuffSize(MAX_PLAYER_SOCKET_BUFF);
	if (0 != pNewSocket->connectServer(mServerIP, mServerPort))
	{
		printf("connect server failure!\n");
		return;
	}

	pNewPlayer->setAccountName(pAccountName);
	pNewPlayer->setChannelID(nChannelID);
	pNewPlayer->setServerID(nServerID);


	mSelectModel.addNewSocket(pNewSocket, nSocketIndex);
	std::pair<PLAYER_SOCKET_LIST::iterator, bool> tResult = mPlayerSocketList.insert(std::pair<int,CPlayer*>(nSocketIndex, pNewPlayer));
	if (!tResult.second)
	{
		printf("add to player socket list failure!\n");
		return;
	}
	pNewPlayer->setTcpIndex(nSocketIndex);
	pNewPlayer->loginServer();
}

uint64 CLoginTester::getTickCount()
{
#ifdef MYTH_OS_WINDOWS
	return GetTickCount64();
#else
	timespec tv;
	// This is not affected by system time changes.
	if (clock_gettime(CLOCK_MONOTONIC, &tv) != 0)
	{
		printf("clock_gettime return error!\n");
		::exit(-1);
	}
	return ((sint64)tv.tv_sec) * 1000 + (((sint64)tv.tv_nsec/*+500*/) / 1000000);
#endif
}

void CLoginTester::onTime(int nElapseTime)
{
	if (mAccountNameCount > 150)
	{
		return;
	}
	int nRand = rand() % 300;

	if (mResetTimer.elapse(nElapseTime))
	{
		mResetTimer.setLeftTime(100000000);
		for (int i = 0; i < 1; ++ i)
		{
			char szAccountName[MAX_NAME_LENGTH] = {0};
			snprintf(szAccountName, sizeof(szAccountName) - 1, "hjh%d", nRand);
			newPlayer(szAccountName, 1, 1);
			++ mAccountNameCount;
		}
	}
}