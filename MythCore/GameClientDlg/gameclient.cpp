#include "stdafx.h"
#include "gameclient.h"
#include "log.h"
#include "logmanager.h"
#include "logdisplayer.h"
#include "loginmessage.hxx.pb.h"
#include "propertymodule.hxx.pb.h"
#include "../GameServer/inc/messagefactory.h"
#include "GameClientDlgDlg.h"
CGameClient::CGameClient()
	:mSelectModel(&mTcpSocket, 1)
{

}

/// ��ʼ��
bool CGameClient::init()
{
	CMessageFactory::createInst();
	CSelectModel::initSocketSystem();
	mTcpSocket.createSocket();
	int nResult = mTcpSocket.connectServer("127.0.0.1", 6688);
	//int nResult = mTcpSocket.connectServer("106.13.146.178", 6688);
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
	CLogManager* pLogManger = CLogManager::createInst();
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
	mSelectModel.selectAllFd();
	processServerMessage();
}

void CGameClient::LoginServer(char *pAccountName, char* pRoleName)
{
	if (NULL == pAccountName || NULL == pRoleName)
	{
		return;
	}
	strncpy(mAccountName, pAccountName, sizeof(mAccountName) - 1);
	strncpy(mRoleName, pRoleName, sizeof(mRoleName) - 1);

	CLoginRequest tLoginRequest;
	tLoginRequest.set_name(pAccountName);
	tLoginRequest.set_channelid(1);
	tLoginRequest.set_serverid(1);
	sendMessage(ID_C2S_REQUEST_LOGIN, &tLoginRequest);
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
				printf("disconnect with server!!!");
				break;
			}
			else
			{
				pAllSocket[i].setRecvBuffSize(pAllSocket[i].getRecvBuffSize() + nResult);
				onServerMessage(&pAllSocket[i]);
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
	for (int i = 0; i < 200; ++ i)
	{
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
		Message* pMessage = CMessageFactory::Inst()->createClientMessage(nMessageID);
		if (NULL != pMessage)
		{
			pMessage->ParseFromArray(pTemp, nLength - sizeof(unsigned short) * 2);
			pTcpSocket->resetRecvBuffPoint(nLength);
			switch (nMessageID)
			{

			case ID_S2C_RESPONSE_LOGIN:
			{
				onMessageLoginResponse(pMessage);
				break;
			}
			case ID_S2C_RESPONSE_CREATE_ROLE:
			{
				onCreateRoleResponse(pMessage);
				break;
			}
			default:
				const ::google::protobuf::Descriptor* pDescriptor = pMessage->GetDescriptor();
				printf("---- Receive from server Msg[ %s ][id: 0x%04x/%d] ---", pDescriptor->name().c_str(), nMessageID, nMessageID);
				printf("[%s]", pMessage->ShortDebugString().c_str());
				mDlg->DisplayLog(const_cast<char*>(pMessage->ShortDebugString().c_str()));
				break;
			}

		}
	}
}

/// ������Ϣ��������
void CGameClient::sendMessage(unsigned short uMessageID, Message* pMessage)
{
	printf("sendMessage ID: %d\n", uMessageID);
	// һ�� unsigned short����ϢID����һ������Ϣ����
	unsigned short nMessageLen = pMessage->ByteSize() + sizeof(unsigned short) * 2;
	byte* pTemp = mBuffer;
	memcpy(pTemp, &nMessageLen, sizeof(nMessageLen));
	pTemp += sizeof(nMessageLen);
	memcpy(pTemp, &uMessageID, sizeof(uMessageID));
	pTemp += sizeof(uMessageID);

	pMessage->SerializeToArray(pTemp, sizeof(mBuffer) - (sizeof(unsigned short) * 2));
	mTcpSocket.sendData(mBuffer, nMessageLen);
}

void CGameClient::onMessageLoginResponse(Message* pMessage)
{
	char acName[256] = { 0 };
	CLoginResponse* pLoginResponse = (CLoginResponse*)pMessage;
	snprintf(acName, sizeof(acName), mRoleName);

	int nRoleID = pLoginResponse->roleid();

	mAccountID = pLoginResponse->accountid();
	mChannelID = pLoginResponse->channelid();
	mServerID = pLoginResponse->serverid();
	mRoleID = pLoginResponse->roleid();
	if (0 == nRoleID)
	{
		// ������
		CCreateRoleRequest tCreateRoleRequest;
		tCreateRoleRequest.set_accountid(pLoginResponse->accountid());
		tCreateRoleRequest.set_channelid(pLoginResponse->channelid());
		tCreateRoleRequest.set_serverid(pLoginResponse->serverid());
		tCreateRoleRequest.set_rolename(acName);
		sendMessage(ID_C2S_REQUEST_CREATE_ROLE, &tCreateRoleRequest);
	}
	else
	{
		CEnterSceneRequest tEnterSceneRequest;
		tEnterSceneRequest.set_roleid(pLoginResponse->roleid());
		tEnterSceneRequest.set_accountid(pLoginResponse->accountid());
		tEnterSceneRequest.set_channelid(pLoginResponse->channelid());
		tEnterSceneRequest.set_serverid(pLoginResponse->serverid());
		sendMessage(ID_C2S_REQUEST_ENTER_SCENE, &tEnterSceneRequest);
	}
}

void CGameClient::onCreateRoleResponse(Message* pMessage)
{
	CCreateRoleResponse* pRoleResponse = (CCreateRoleResponse*)pMessage;

	int nResult = pRoleResponse->result();
	unsigned int nRoleID = pRoleResponse->roleid();
	mRoleID = nRoleID;

	CEnterSceneRequest tEnterSceneRequest;
	tEnterSceneRequest.set_roleid(mRoleID);
	tEnterSceneRequest.set_accountid(mAccountID);
	tEnterSceneRequest.set_channelid(mChannelID);
	tEnterSceneRequest.set_serverid(mServerID);
	sendMessage(ID_C2S_REQUEST_ENTER_SCENE, &tEnterSceneRequest);

}

void CGameClient::sendGMCommandRequest(char* pCommandString)
{
	if (NULL == pCommandString)
	{
		return;
	}
	CGMCommandRequest tRequest;
	tRequest.set_commandstring(pCommandString);
	sendMessage(ID_C2S_REQUEST_GM_COMMAND, &tRequest);
}
