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
	
		default:
			break;
	}
}