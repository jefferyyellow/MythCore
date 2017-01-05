#include "locallogjob.h"
#include "gameserver.h"
#include "logmanager.h"
#include "internalmsgpool.h"
#include "internalmsg.h"
void LogLocalLog(EmLogType eLogType, const char* pFormat, ...)
{
	va_list valist;
	va_start(valist, pFormat);
	char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
	vsnprintf(tBuffer, sizeof(tBuffer) - 1, pFormat, valist);
	va_end(valist);
	
	CIMLocalLogRequest* pLocalLogRequest = reinterpret_cast<CIMLocalLogRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_LOCAL_LOG));
	if (NULL != pLocalLogRequest)
	{
		pLocalLogRequest->mLogType = eLogType;
		strncpy(pLocalLogRequest->mLogContent, tBuffer, sizeof(pLocalLogRequest->mLogContent));
		CGameServer::Inst()->pushTask(emTaskType_LocalLog, pLocalLogRequest);
	}
}

void LogLocalDebugLog(const char* pLogName, const char* pFormat, ...)
{
	va_list valist;
	va_start(valist, pFormat);
	char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
	vsnprintf(tBuffer, sizeof(tBuffer) - 1, pFormat, valist);
	va_end(valist);

	CIMLocalLogRequest* pLocalLogRequest = reinterpret_cast<CIMLocalLogRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_LOCAL_LOG));
	if (NULL != pLocalLogRequest)
	{
		pLocalLogRequest->mLogType = emLogType_Debug;
		strncpy(pLocalLogRequest->mDebugName, pLogName, sizeof(pLocalLogRequest->mDebugName));
		strncpy(pLocalLogRequest->mLogContent, tBuffer, sizeof(pLocalLogRequest->mLogContent));
		CGameServer::Inst()->pushTask(emTaskType_LocalLog, pLocalLogRequest);
	}
}


void CLocalLogJob::doing(uint32 uParam)
{
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}
		
		if (pIMMsg->getMsgID() == IM_REQUEST_LOCAL_LOG)
		{
			OnIMLocalLogRequest(reinterpret_cast<CIMLocalLogRequest*>(pIMMsg));
		}
		else
		{
			// 出错了
		}

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
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