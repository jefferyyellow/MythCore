#include "locallogjob.h"
#include "gameserver.h"
#include "logmanager.h"
#include "internalmsgpool.h"
#include "internalmsg.h"
void LogLocalLog(EmLogType eLogType, const char* pFile, int nLine, const char* pFunction, const char* pFormat, ...)
{
	CIMLocalLogRequest* pLocalLogRequest = static_cast<CIMLocalLogRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_LOCAL_LOG));
	if (NULL == pLocalLogRequest)
	{
		return;
	}
	const char* pFileName = NULL;
	va_list valist;
	va_start(valist, pFormat);
	int nSize = snprintf(pLocalLogRequest->mLogContent, sizeof(pLocalLogRequest->mLogContent)-1, "[%s:%d (%s)] ", LOG_FILE(pFile), nLine, pFunction);
	vsnprintf(pLocalLogRequest->mLogContent + nSize, sizeof(pLocalLogRequest->mLogContent)-nSize - 1, pFormat, valist);
	va_end(valist);
	pLocalLogRequest->mLogType = eLogType;
	CGameServer::Inst()->pushTask(emTaskType_LocalLog, pLocalLogRequest);
}

void LogNoLocation(EmLogType eLogType, const char* pFormat, ...)
{
	CIMLocalLogRequest* pLocalLogRequest = static_cast<CIMLocalLogRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_LOCAL_LOG));
	if (NULL == pLocalLogRequest)
	{
		return;
	}
	const char* pFileName = NULL;
	va_list valist;
	va_start(valist, pFormat);
	vsnprintf(pLocalLogRequest->mLogContent, sizeof(pLocalLogRequest->mLogContent) - 1, pFormat, valist);
	va_end(valist);


	pLocalLogRequest->mLogType = eLogType;
	CGameServer::Inst()->pushTask(emTaskType_LocalLog, pLocalLogRequest);
}

void LogLocalDebugLog(const char* pLogName, const char* pFile, int nLine, const char* pFunction, const char* pFormat, ...)
{
	CIMLocalLogRequest* pLocalLogRequest = static_cast<CIMLocalLogRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_LOCAL_LOG));
	if (NULL == pLocalLogRequest)
	{
		return;
	}
	const char* pFileName = NULL;

	pLocalLogRequest->mLogType = emLogType_Debug;
	strncpy(pLocalLogRequest->mDebugName, pLogName, sizeof(pLocalLogRequest->mDebugName));
	va_list valist;
	va_start(valist, pFormat);
	int nSize = snprintf(pLocalLogRequest->mLogContent, sizeof(pLocalLogRequest->mLogContent)-1, "[%s:%d (%s)] ", LOG_FILE(pFile), nLine, pFunction);
	vsnprintf(pLocalLogRequest->mLogContent + nSize, sizeof(pLocalLogRequest->mLogContent)-nSize - 1, pFormat, valist);
	va_end(valist);
	CGameServer::Inst()->pushTask(emTaskType_LocalLog, pLocalLogRequest);
}

void CLocalLogJob::init()
{
    mLastTime = 0;
}

void CLocalLogJob::initAll()
{
	mLastTime = CTimeManager::Inst()->getCurrTime();
}

void CLocalLogJob::doing(int uParam)
{
	int nCount = 0;
	//Sleep(100);
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// Èç¹û
		if (NULL == pIMMsg)
		{
			break;
		}
		
		if (pIMMsg->getMsgID() == IM_REQUEST_LOCAL_LOG)
		{
			OnIMLocalLogRequest(static_cast<CIMLocalLogRequest*>(pIMMsg));
		}

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
		++ nCount;
		if (nCount > 1000)
		{
			break;
		}
	}

	uint64 tTimeNow = CTimeManager::Inst()->getCurrTime();
	if (tTimeNow != mLastTime)
	{
		CLogManager::Inst()->setTmNow(tTimeNow);
		mLastTime = tTimeNow;
	}
}

void CLocalLogJob::OnIMLocalLogRequest(CIMLocalLogRequest* pLogRequest)
{
	if (NULL == pLogRequest)
	{
		return;
	}

	switch (pLogRequest->mLogType)
	{
		case emLogType_Error:
		{
			CLogManager::Inst()->LogErrorMessage(pLogRequest->mLogContent);
			break;
		}
		case emLogType_Info:
		{
			CLogManager::Inst()->LogInfoMessage(pLogRequest->mLogContent);
			break;
		}
		case emLogType_Warn:
		{
			CLogManager::Inst()->LogWarnMessage(pLogRequest->mLogContent);
			break;
		}
		case emLogType_Debug:
		{
			CLogManager::Inst()->LogDebugMessage(pLogRequest->mDebugName, pLogRequest->mLogContent);
			break;
		}
		default:
			break;
	}
}