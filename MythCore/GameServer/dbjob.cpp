#include "dbjob.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "mysqlqueryresult.h"
#include "gameserver.h"
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

void CDBJob::doing(uint32 uParam)
{
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


void CDBJob::onTask(CInternalMsg* pMsg)
{
	switch (pMsg->getMsgID())
	{
		case IM_REQUEST_PLAYER_LOGIN:
		{
			OnIMPlayerLoginRequest(pMsg);
			break;
		}
		default:
			break;
	}
}

/// 玩家登陆游戏
void CDBJob::OnIMPlayerLoginRequest(CInternalMsg* pMsg)
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
	snprintf(acBuffer, sizeof(acBuffer), "call CheckUserName('%s', %d, %d)", pLoginRequest->mName, pLoginRequest->mChannelID, pLoginRequest->mWorldID);

	CMysqlQueryResult tQueryResult;
	mDataBase.query(acBuffer, tQueryResult);

	if (tQueryResult.getRowCount() != 1)
	{
		return;
	}

	uint32 nAccountID = atoi(tQueryResult.getField(0)->getValue());
	if (nAccountID <= 0)
	{
		return;
	}
	
	snprintf(acBuffer, sizeof(acBuffer), "call GetRoleInfo(%d, %d, %d)", nAccountID, pLoginRequest->mChannelID, pLoginRequest->mWorldID);
	tQueryResult.clear();
	mDataBase.clearResult();

	mDataBase.query(acBuffer, tQueryResult);
	if (tQueryResult.getRowCount() != 1)
	{
		mDataBase.clearResult();
		return;
	}

	uint32 nRoleID = atoi(tQueryResult.getField(0)->getValue());

	CIMPlayerLoginResponse* pResponse = reinterpret_cast<CIMPlayerLoginResponse*>(CInternalMsgPool::CreateInst()->allocMsg(IM_RESPONSE_PLAYER_LOGIN));
	if (NULL != pResponse)
	{
		pResponse->mAccountID = nAccountID;
		pResponse->mRoleID = nRoleID;
		pResponse->mChannelID = pLoginRequest->mChannelID;
		pResponse->mWorldID = pLoginRequest->mWorldID;
		pResponse->mSocketIndex = pLoginRequest->mSocketIndex;
		pResponse->mSocketTime = pLoginRequest->mSocketTime;
		CGameServer::Inst()->pushTask(emTaskType_Login, pResponse);
		printf("OnIMPlayerLoginRequest end");
	}
	mDataBase.clearResult();
}