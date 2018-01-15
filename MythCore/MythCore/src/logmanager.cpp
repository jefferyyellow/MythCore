#include "logmanager.h"
#include "log.h"
#include <stdio.h>
#ifdef MYTH_OS_UNIX
#include <time.h>
#endif

namespace Myth
{
	int CLogManager::Init()
	{
		return 0;
	}

	void CLogManager::Finial()
	{

	}

	/// log error message
	void CLogManager::LogErrorMessageFormat(const char* pFormat, ...)
	{
		va_list valist;
		va_start(valist, pFormat);

		char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
		FormatLogMessage(tBuffer, sizeof(tBuffer) - 1, "ERROR", pFormat, valist);
		mErrorLog.DisplayLog(tBuffer);
		va_end(valist);
	}

	void CLogManager::LogErrorMessage(const char* pLogContent)
	{
		char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
		FormatLogMessage(tBuffer, sizeof(tBuffer) - 1, "ERROR", pLogContent);
		mErrorLog.DisplayLog(tBuffer);
	}


	/// log info message
	void CLogManager::LogInfoMessageFormat(const char* pFormat, ...)
	{
		va_list valist;
		va_start(valist, pFormat);

		char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
		FormatLogMessage(tBuffer, sizeof(tBuffer) - 1, "INFO", pFormat, valist);
		mInfoLog.DisplayLog(tBuffer);

		va_end(valist);
	}

	void CLogManager::LogInfoMessage(const char* pLogContent)
	{
		char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
		FormatLogMessage(tBuffer, sizeof(tBuffer) - 1, "INFO", pLogContent);
		mInfoLog.DisplayLog(tBuffer);
	}

	/// log warnning message
	void CLogManager::LogWarnMessageFormat(const char* pFormat, ...)
	{
		va_list valist;
		va_start(valist, pFormat);

		char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
		FormatLogMessage(tBuffer, sizeof(tBuffer) - 1, "WARN", pFormat, valist);
		mWarnLog.DisplayLog(tBuffer);

		va_end(valist);
	}

	void CLogManager::LogWarnMessage(const char* pLogContent)
	{
		char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
		FormatLogMessage(tBuffer, sizeof(tBuffer) - 1, "WARN", pLogContent);
		mWarnLog.DisplayLog(tBuffer);
	}

	/// Get the specified name log
	CLog* CLogManager::GetDebugLog(const char* pName)
	{
		DebugLogMap::iterator it = mDebugLog.find(pName);
		if (it != mDebugLog.end())
		{
			return it->second;
		}
		return NULL;
	}

	/// add a new specified name debug log
	void CLogManager::AddDebugLog(CLog* pLog, const char* pName)
	{
		if (NULL == pLog || NULL == pName)
		{
			return;
		}

		mDebugLog[pName] = pLog;
		return;
	}

	/// log debug message
	void CLogManager::LogDebugMessageFormat(const char* pLogName, const char* pFormat, ...)
	{
		va_list valist;
		va_start(valist, pFormat);

		char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
		FormatLogMessage(tBuffer, sizeof(tBuffer) - 1, "DEBUG", pFormat, valist);
		CLog* pErrorLog = GetDebugLog(pLogName);
		if (NULL != pErrorLog)
		{
			pErrorLog->DisplayLog(tBuffer);
		}

		va_end(valist);
	}

	void CLogManager::LogDebugMessage(const char* pLogName, const char* pLogContent)
	{
		char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
		FormatLogMessage(tBuffer, sizeof(tBuffer) - 1, "DEBUG", pLogContent);
		CLog* pErrorLog = GetDebugLog(pLogName);
		if (NULL != pErrorLog)
		{
			pErrorLog->DisplayLog(tBuffer);
		}
	}

	/// format date into data buffer
	void CLogManager::FormatDateString(char* pDateBuffer, int nBuffSize)
	{
		time_t date;
		time(&date);
		tm* tms = localtime(&date);
		if (NULL != tms)
		{
			snprintf(pDateBuffer, nBuffSize - 1, "[%04d-%02d-%02d %02d:%02d:%02d]", tms->tm_year + 1900,
				tms->tm_mon + 1, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);
			pDateBuffer[nBuffSize - 1] = '\0';
		}
	}

	/// format log int data buffer
	void CLogManager::FormatLogMessage(char* pDataBuffer, int nBuffSize, const char* pLogTypeName, const char* pFormat, va_list vaList)
	{
		char tBuffer[MAX_LOG_BUFFER_NUM] = { 0 };
		vsnprintf(tBuffer, sizeof(tBuffer) - 1, pFormat, vaList);

		char tDateBuffer[STRING_LENGTH_32] = { 0 };
		FormatDateString(tDateBuffer, sizeof(tDateBuffer));

		snprintf(pDataBuffer, nBuffSize - 1, "%s %s: %s\n", tDateBuffer, pLogTypeName, tBuffer);
	}

	void CLogManager::FormatLogMessage(char* pDataBuffer, int nBuffSize, const char* pLogTypeName, const char* pLogContent)
	{
		char tDateBuffer[STRING_LENGTH_32] = { 0 };
		FormatDateString(tDateBuffer, sizeof(tDateBuffer));

		snprintf(pDataBuffer, nBuffSize - 1, "%s %s: %s\n", tDateBuffer, pLogTypeName, pLogContent);

	}
}

