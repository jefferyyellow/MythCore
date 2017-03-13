#include "scenejob.h"
#include "messagefactory.h"
#include "loginmessage.hxx.pb.h"
#include "message.hxx.pb.h"
#include "loginmodule.h"
#include "internalmsgpool.h"
#include "entityplayer.h"

void CSceneJob::doing(uint32 uParam)
{
	processClientMessage();
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}
		onTask(pIMMsg);

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
	}
}

void CSceneJob::onTask(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	switch (pMsg->getMsgID())
	{
		case IM_RESPONSE_PLAYER_LOGIN:
		{
			CLoginModule::Inst()->onIMPlayerLoginResponse(pMsg);
			break;
		}
		case IM_RESPONSE_CREATE_ROLE:
		{
			CLoginModule::Inst()->onIMCreateRoleResponse(pMsg);
			break;
		}
		case IM_RESPONSE_ENTER_SCENE:
		{
			CLoginModule::Inst()->onIMEnterSceneResponse(pMsg);
			break;
		}
		default:
			break;
	}
}

bool CSceneJob::init()
{
	bool bResult = initShareMemory();
	if (!bResult)
	{
		return false;
	}
	
	return true;
}

/// 初始化管道
bool CSceneJob::initShareMemory()
{
	// 一个CShareMemory管理类
	int nShareMemorySize = sizeof(CShareMemory);
	// 两个CSocketStream管理类
	nShareMemorySize += 2 * sizeof(CSocketStream);
	// 两个真实的内存区域
	nShareMemorySize += 2 * PIPE_SIZE;

	bool bCreate = true;
	uint8* pSharePoint = CShareMemory::createShareMemory(37345234, nShareMemorySize, bCreate);
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

/// 处理前端消息
void CSceneJob::processClientMessage()
{
	int nMessageLen = MAX_SOCKET_BUFF_SIZE;
	int nResult = 0;
	for (int i = 0; i < 2000; ++i)
	{
		nResult = mTcp2ServerMemory->GetHeadPacket((uint8*)mBuffer, nMessageLen);
		if (nResult < 0)
		{
			break;
		}

		if (nMessageLen <= 0)
		{
			break;
		}

		char* pTemp = mBuffer;
		CExchangeHead* pExchangeHead = (CExchangeHead*)mBuffer;

		pTemp += sizeof(CExchangeHead);
		nMessageLen -= sizeof(CExchangeHead);

		short nLength = *(short*)pTemp;
		if (nLength != nMessageLen)
		{
			break;
		}

		pTemp += sizeof(short);
		nMessageLen -= sizeof(short);

		short nMessageID = *(short*)pTemp;
		pTemp += sizeof(short);
		nMessageLen -= sizeof(short);

		Message* pMessage = CMessageFactory::Inst()->createClientMessage(nMessageID);
		if (NULL != pMessage)
		{
			pMessage->ParseFromArray(pTemp, nMessageLen);
			int nModule = nMessageID & MESSAGE_MODULE_MASK;
			if (nModule == MESSAGE_MODULE_LOGIN)
			{
				CLoginModule::Inst()->onClientMessage(pExchangeHead->mSocketIndex, nMessageID, pMessage);
			}
			else
			{
				dispatchClientMessage(nMessageID, pMessage);
			}
		}
	}
}

/// 发送前端消息
void CSceneJob::sendClientMessage(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage)
{
	char* pTemp = mBuffer;
	memcpy(pTemp, &rExchangeHead, sizeof(rExchangeHead));
	pTemp += sizeof(rExchangeHead);

	unsigned short nMessageLen = pMessage->ByteSize() + sizeof(unsigned short) * 2;
	memcpy(pTemp, &nMessageLen, sizeof(nMessageLen));

	pTemp += sizeof(nMessageLen);

	memcpy(pTemp, &nMessageID, sizeof(nMessageID));
	pTemp += sizeof(nMessageID);

	pMessage->SerializeToArray(pTemp, sizeof(mBuffer) - sizeof(rExchangeHead) - sizeof(unsigned short) * 2);
	printf("PushPacket");
	mServer2TcpMemory->PushPacket((uint8*)mBuffer, pMessage->ByteSize() + sizeof(rExchangeHead) + sizeof(unsigned short) * 2);

}

void CSceneJob::sendClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	sendClientMessage(pPlayer->GetExhangeHead(), nMessageID, pMessage);
}

/// 分发前端消息
void CSceneJob::dispatchClientMessage(unsigned short nMessageID, Message* pMessage)
{
	int nModule = nMessageID & MESSAGE_MODULE_MASK;
	switch (nModule)
	{
		default:
			break;
	}
}

