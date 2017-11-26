#include "dbjob.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "mysqlqueryresult.h"
#include "gameserver.h"
#include "entityplayer.h"
#include "objpool.h"
CDBJob::CDBJob()
{

}

CDBJob::~CDBJob()
{

}

int CDBJob::init(char* pHost, char* pUserName, char* pPasswd, char* pDataBase, int nPort, char* pUnixSocket)
{
	int nResult = mDataBase.connectMysqlServer(pHost, pUserName, pPasswd, pDataBase, nPort, pUnixSocket);
	return nResult;

	return 0;
}

void CDBJob::doing(int uParam)
{
	//printf("CDBJob:: %d\n", uParam);
	//Sleep(3000);
	checkDBStream();
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

int CDBJob::setBuffer(int nBufferSize)
{
	mJobBuffer = new byte[nBufferSize];
	if (NULL == mJobBuffer)
	{
		return -1;
	}
	mJobStream.Initialize(mJobBuffer, nBufferSize);
	return 0;
}

/// 压入工作数据
void CDBJob::pushBackJobData(byte* pData, int nDataLength)
{
	if (NULL == pData || 0 == nDataLength)
	{
		return;
	}

	mJobStreamLock.lock();
	mJobStream.PushPacket(pData, nDataLength);
	mJobStreamLock.unlock();
}

/// 取出工作数据
void CDBJob::popUpJobData(byte* pData, int &rLength)
{
	if (NULL == pData)
	{
		return;
	}

	mJobStreamLock.lock();
	mJobStream.GetHeadPacket(pData, rLength);
	mJobStreamLock.unlock();
}

/// 处理DB流里的数据
void CDBJob::checkDBStream()
{
	for (int i = 0; i < 2000; ++ i)
	{
		int nLength = 0;
		popUpJobData((byte*)&mDBRequest, nLength);
		if (nLength <= 0)
		{
			return;
		}

		int nResultLength = sizeof(mDBResponse.mSqlBuffer) - 1;

		int nRowNum = 0;
		int nColNum = 0;
		int nResult = mDataBase.query((char*)mDBRequest.mSqlBuffer, (byte*)mDBResponse.mSqlBuffer, nResultLength,
			nRowNum, nColNum);


		mDBResponse.mPlayerID = mDBRequest.mPlayerID;
		mDBResponse.mResult = nResult;
		mDBResponse.mParam1 = mDBRequest.mParam1;
		mDBResponse.mParam2 = mDBRequest.mParam2;
		mDBResponse.mSessionType = mDBRequest.mSessionType;
		mDBResponse.mRowNum = nRowNum;
		mDBResponse.mColNum = nColNum;
		mDBResponse.mSqlLenth = nResultLength;
		CSceneJob::Inst()->pushBackDBData((byte*)&mDBResponse, sizeof(CDBResponseHeader)+nResultLength);
	}
}

void CDBJob::onTask(CInternalMsg* pMsg)
{
	switch (pMsg->getMsgID())
	{
		case IM_REQUEST_PLAYER_LOGIN:
		{
			onIMPlayerLoginRequest(pMsg);
			break;
		}
		case IM_REQUEST_CREATE_ROLE:
		{
			onIMCreateRoleRequest(pMsg);
			break;
		}
		case IM_REQUEST_ENTER_SCENE:
		{
			onIMEnterSceneRequest(pMsg);
			break;
		}
		default:
			break;
	}
}

/// 玩家登陆游戏
void CDBJob::onIMPlayerLoginRequest(CInternalMsg* pMsg)
{
	printf("OnIMPlayerLoginRequest begin");
	if (NULL == pMsg)
	{
		return;
	}

	CIMPlayerLoginRequest* pLoginRequest = reinterpret_cast<CIMPlayerLoginRequest*>(pMsg);
	if (NULL == pLoginRequest)
	{
		return;
	}
	// 得到account id
	char acBuffer[STRING_LENGTH_128] = {0};
	snprintf(acBuffer, sizeof(acBuffer), "call CheckUserName('%s', %d, %d)", pLoginRequest->mName, pLoginRequest->mChannelID, pLoginRequest->mServerID);

	CMysqlQueryResult tQueryResult(&mDataBase, true);
	mDataBase.query(acBuffer, tQueryResult);

	if (tQueryResult.getRowCount() != 1)
	{
		return;
	}

	unsigned int nAccountID = atoi(tQueryResult.getField(0)->getValue());
	if (nAccountID <= 0)
	{
		return;
	}
	
	snprintf(acBuffer, sizeof(acBuffer), "call GetRoleInfo(%d, %d, %d)", nAccountID, pLoginRequest->mChannelID, pLoginRequest->mServerID);
	tQueryResult.clear();

	mDataBase.query(acBuffer, tQueryResult);
	if (tQueryResult.getRowCount() != 1)
	{
		return;
	}

	unsigned int nRoleID = atoi(tQueryResult.getField(0)->getValue());

	CIMPlayerLoginResponse* pResponse = reinterpret_cast<CIMPlayerLoginResponse*>(CInternalMsgPool::CreateInst()->allocMsg(IM_RESPONSE_PLAYER_LOGIN));
	if (NULL == pResponse)
	{
		return;
	}

	copyPlayerLoginMsg(pResponse, pLoginRequest);
	pResponse->mRoleID = nRoleID;
	pResponse->mAccountID = nAccountID;
	strncpy(pResponse->mName, pLoginRequest->mName, MAX_PLAYER_NAME_LEN);


	CGameServer::Inst()->pushTask(emTaskType_Scene, pResponse);
	printf("OnIMPlayerLoginRequest end");
}

/// 玩家创建角色
void CDBJob::onIMCreateRoleRequest(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}

	CIMCreateRoleRequest* pCreateRoleRequest = reinterpret_cast<CIMCreateRoleRequest*>(pMsg);
	if (NULL == pCreateRoleRequest)
	{
		return;
	}
	char acBuffer[STRING_LENGTH_128] = { 0 };
	snprintf(acBuffer, sizeof(acBuffer), "call CreateRole(%d, '%s', %d, %d, %d)", 1, pCreateRoleRequest->mRoleName, pCreateRoleRequest->mAccountID, pCreateRoleRequest->mChannelID, pCreateRoleRequest->mServerID);
	CMysqlQueryResult tQueryResult(&mDataBase, true);
	mDataBase.query(acBuffer, tQueryResult);

	if (tQueryResult.getRowCount() != 1)
	{
		return;
	}

	unsigned int nRoleID = atoi(tQueryResult.getField(0)->getValue());
	
	CIMCreateRoleResponse* pIMCreateRoleResponse = reinterpret_cast<CIMCreateRoleResponse*>(CInternalMsgPool::Inst()->allocMsg(IM_RESPONSE_CREATE_ROLE));
	if (NULL == pIMCreateRoleResponse)
	{
		return;
	}

	pIMCreateRoleResponse->mRoleID = nRoleID;
	strncpy(pIMCreateRoleResponse->mRoleName, pCreateRoleRequest->mRoleName, MAX_PLAYER_NAME_LEN - 1);
	copyPlayerLoginMsg(pIMCreateRoleResponse, pCreateRoleRequest);

	CGameServer::Inst()->pushTask(emTaskType_Scene, pIMCreateRoleResponse);
}

// 玩家进入场景
void CDBJob::onIMEnterSceneRequest(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}

	CIMEnterSceneRequest* pEnterSceneRequest = reinterpret_cast<CIMEnterSceneRequest*>(pMsg);
	if (NULL == pEnterSceneRequest)
	{
		return;
	}

	CIMEnterSceneResponse* pEnterSceneResponse = reinterpret_cast<CIMEnterSceneResponse*>(CInternalMsgPool::Inst()->allocMsg(IM_RESPONSE_ENTER_SCENE));
	if (NULL == pEnterSceneResponse)
	{
		return;
	}

	unsigned int nRoleID = pEnterSceneRequest->mRoleID;
	unsigned int nPlayerEntityID = pEnterSceneRequest->mPlayerEntityID;

	char acBuffer[STRING_LENGTH_128] = { 0 };
	snprintf(acBuffer, sizeof(acBuffer), "call LoadPlayerInfo(%d)", nRoleID);
	CMysqlQueryResult tQueryResult(&mDataBase, true);
	mDataBase.query(acBuffer, tQueryResult);

	CEntityPlayer* pEntityPlayer = reinterpret_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(nPlayerEntityID));
	if (NULL == pEntityPlayer)
	{
		return;
	}
	// 取得数据库加载的结果
	pEntityPlayer->setName(tQueryResult.getField(0)->getValue());
	//pEntityPlayer->setRoleLevel(atoi(tQueryResult.getField(1)->getValue()));
	//pEntityPlayer->setRoleExp(atoll(tQueryResult.getField(2)->getValue()));

	pEnterSceneResponse->mRoleID = pEnterSceneRequest->mRoleID;
	pEnterSceneResponse->mPlayerEntityID = pEnterSceneRequest->mPlayerEntityID;

	copyPlayerLoginMsg(pEnterSceneResponse, pEnterSceneRequest);
	CGameServer::Inst()->pushTask(emTaskType_Scene, pEnterSceneResponse);
}

// 拷贝玩家登陆消息
void CDBJob::copyPlayerLoginMsg(CIMPlayerLoginMsg* pDstMsg, CIMPlayerLoginMsg* pSrcMsg)
{
	pDstMsg->mSocketIndex = pSrcMsg->mSocketIndex;
	pDstMsg->mAccountID = pSrcMsg->mAccountID;
	pDstMsg->mChannelID = pSrcMsg->mChannelID;
	pDstMsg->mServerID = pSrcMsg->mServerID;
}