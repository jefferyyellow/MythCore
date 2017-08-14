#include "gameclient.h"
#include "log.h"
#include "logmanager.h"
#include "logdisplayer.h"
#include "loginmessage.hxx.pb.h"
#include "../GameServer/inc/messagefactory.h"

CGameClient::CGameClient()
	:mSelectModel(&mTcpSocket, 1)
{

}

/// ��ʼ��
bool CGameClient::init()
{
	CSelectModel::initSocketSystem();
	mTcpSocket.createSocket();
	int nResult = mTcpSocket.connectServer("127.0.0.1", 6688);
	if (!initLog())
	{
		return false;
	}
	mTcpSocket.setRecvBuff(mTcpRecData);
	mTcpSocket.setRecvBuffSize(0);
	mTcpSocket.setMaxRecvBuffSize(sizeof(mTcpRecData));
	mSelectModel.addNewSocket(&mTcpSocket, 0);
	return true;
}

/// ��ʼ��־
bool CGameClient::initLog()
{
	CLogManager* pLogManger = CLogManager::CreateInst();
	if (NULL == pLogManger)
	{
		printf("create log manager failure");
		return false;
	}

#ifdef __DEBUG__
	// Ĭ����־
	mDefaultLog = new CLog();
	if (NULL == mDefaultLog)
	{
		printf("create default log failure");
		return false;
	}
#endif

	// Ĭ�ϵ�debug��־
	CRollFileDisplayer* pDefaultFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/clientdefault.log"), 1024000, 10);
	// ΪĬ�ϵ�debug��־���ļ�displayer
	mDefaultLog->AddDisplayer(pDefaultFileDisplayer);
	CLogManager::Inst()->AddDebugLog(mDefaultLog, "default");

	// ������־���ļ�displayer
	CRollFileDisplayer* pErrorFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/clienterror.log"), 1024000, 10);
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pErrorFileDisplayer);

	// ������־��std displayer
	CStdDisplayer* pDisplayer = new CStdDisplayer();
	CLogManager::Inst()->GetErrorLog().AddDisplayer(pDisplayer);

	// ����Ϣ��־���ļ�displayer
	CRollFileDisplayer* pInfoFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/clientinfo.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pInfoFileDisplayer);

	// ��������־���ļ�displayer
	CRollFileDisplayer* pWarnFileDisplayer = new CRollFileDisplayer(const_cast<char*>("../log/clientwarn.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pWarnFileDisplayer);
	return true;
}

/// ����
void CGameClient::run()
{
	LoginServer();
	while (true)
	{
		mSelectModel.selectAllFd();
		processServerMessage();
		Sleep(30);
	}
}

void CGameClient::LoginServer()
{
	for (int i = 0; i < 1000; ++ i)
	{
		CMessageLoginRequest tLoginRequest;
		tLoginRequest.set_name("hjh");
		tLoginRequest.set_channelid(1);
		tLoginRequest.set_serverid(1);
		sendMessage(ID_C2S_REQUEST_LOGIN, &tLoginRequest);
		Sleep(1000);
	}

}

/// ����ǰ����Ϣ
void CGameClient::processServerMessage()
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
				// �ͻ����Ѿ��˳�
				int nRemoveFd = pAllSocket[i].getSocketFd();
				pAllSocket[i].closeSocket();
				mSelectModel.removeSocket(nRemoveFd);
				break;
			}
			else
			{
				pAllSocket[i].setRecvBuffSize(pAllSocket[i].getRecvBuffSize() + nResult);
				onServerMessage(&pAllSocket[i]);
				printf("receive message");
			}
		}
	}
}

/// ���յ���������Ϣ
void CGameClient::onServerMessage(CTcpSocket* pTcpSocket)
{
	if (NULL == pTcpSocket)
	{
		return;
	}

	char* pTemp = pTcpSocket->getRecvBuff();
	if (pTcpSocket->getRecvBuffSize() < sizeof(unsigned short) * 2)
	{
		return;
	}

	short nLength = *(short*)pTemp;
	pTemp += sizeof(short);

	short nMessageID = *(short*)pTemp;
	pTemp += sizeof(short);

	Message* pMessage = CMessageFactory::Inst()->createClientMessage(nMessageID);
	if (NULL != pMessage)
	{
		pMessage->ParseFromArray(pTemp, nLength - sizeof(unsigned short) * 2);
		switch (nMessageID)
		{
			case ID_S2C_RESPONSE_LOGIN:
			{
				onMessageLoginResponse(pMessage);
				break;
			}
			default:
				break;
		}

	}
}

/// ������Ϣ��������
void CGameClient::sendMessage(unsigned short uMessageID, Message* pMessage)
{
	// һ�� unsigned short����ϢID����һ������Ϣ����
	unsigned short nMessageLen = pMessage->ByteSize() + sizeof(unsigned short) * 2;
	char* pTemp = mBuffer;
	memcpy(pTemp, &nMessageLen, sizeof(nMessageLen));
	pTemp += sizeof(nMessageLen);
	memcpy(pTemp, &uMessageID, sizeof(uMessageID));
	pTemp += sizeof(uMessageID);

	pMessage->SerializeToArray(pTemp, sizeof(mBuffer) - (sizeof(unsigned short) * 2));
	mTcpSocket.sendData(mBuffer, nMessageLen);
}

void CGameClient::onMessageLoginResponse(Message* pMessage)
{
	CMessageLoginResponse* pLoginResponse = (CMessageLoginResponse*)pMessage;
	int nRoleID = pLoginResponse->roleid();
	if (0 == nRoleID)
	{
		// ������
		CMessageCreateRoleRequest tCreateRoleRequest;
		tCreateRoleRequest.set_accountid(pLoginResponse->accountid());
		tCreateRoleRequest.set_channelid(pLoginResponse->channelid());
		tCreateRoleRequest.set_serverid(pLoginResponse->serverid());
		tCreateRoleRequest.set_rolename("hjh");
		sendMessage(ID_C2S_REQUEST_CREATE_ROLE, &tCreateRoleRequest);
	}
	else
	{
		CMessageEnterSceneRequest tEnterSceneRequest;
		tEnterSceneRequest.set_roleid(pLoginResponse->roleid());
		tEnterSceneRequest.set_accountid(pLoginResponse->accountid());
		tEnterSceneRequest.set_channelid(pLoginResponse->channelid());
		tEnterSceneRequest.set_serverid(pLoginResponse->serverid());
		sendMessage(ID_C2S_REQUEST_ENTER_SCENE, &tEnterSceneRequest);
	}
}
