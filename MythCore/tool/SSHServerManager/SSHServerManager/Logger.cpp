#include "stdafx.h"
#include "Logger.h"
#include <io.h>

void LogDebugFFL	(char* pFile, char* pFunction, int nLine, const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);
	CLogger::Inst()->SaveLogFFL(CLogger::emLevel_Debug, pFile, pFunction, nLine, vFmt, valist);
	va_end(valist);
}

void LogInfoFFL		(char* pFile, char* pFunction, int nLine, const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);

	CLogger::Inst()->SaveLogFFL(CLogger::emLevel_Info, pFile, pFunction, nLine, vFmt, valist);
	va_end(valist);
}

void LogNoticeFFL	(char* pFile, char* pFunction, int nLine, const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);

	CLogger::Inst()->SaveLogFFL(CLogger::emLevel_Noice, pFile, pFunction, nLine, vFmt, valist);
	va_end(valist);
}

void LogWarnFFL		(char* pFile, char* pFunction, int nLine, const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);
	CLogger::Inst()->SaveLogFFL(CLogger::emLevel_Warn, pFile, pFunction, nLine, vFmt, valist);
	va_end(valist);
}

void LogErrorFFL	(char* pFile, char* pFunction, int nLine, const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);
	CLogger::Inst()->SaveLogFFL(CLogger::emLevel_Error, pFile, pFunction, nLine, vFmt, valist);
	va_end(valist);
}

void LogFatalFFL	(char* pFile, char* pFunction, int nLine, const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);
	CLogger::Inst()->SaveLogFFL(CLogger::emLevel_Fatal, pFile, pFunction, nLine, vFmt, valist);
	va_end(valist);
}

void LogDebug			(const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);
	CLogger::Inst()->SaveLog(CLogger::emLevel_Debug, vFmt, valist);
	va_end(valist);
}

void LogInfo			(const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);
	CLogger::Inst()->SaveLog(CLogger::emLevel_Info, vFmt, valist);
	va_end(valist);
}

void LogNotice			(const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);

	CLogger::Inst()->SaveLog(CLogger::emLevel_Noice, vFmt, valist);
	va_end(valist);
}

void LogWarn			(const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);

	CLogger::Inst()->SaveLog(CLogger::emLevel_Warn, vFmt, valist);
	va_end(valist);
}

void LogError			(const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);

	CLogger::Inst()->SaveLog(CLogger::emLevel_Error, vFmt, valist);
	va_end(valist);
}

void LogFatal			(const char* vFmt, ... )
{
	va_list valist;
	va_start(valist, vFmt);
	CLogger::Inst()->SaveLog(CLogger::emLevel_Fatal, vFmt, valist);
	va_end(valist);
}

/**************************************************************/
CLogger* CLogger::m_pInst = NULL;
CHAR* g_pLogFileName[] =
{
	"fatal",		//0		
	"error",		//1		
	"warn",			//2		
	"notice",		//3		
	"info",			//4		
	"debug",		//5		
	"notset",		//6		
};

CLogger::CLogger(char* pLogFileName)
{
	m_bScreen = false;

	// 保存文件名
	strncpy_s(m_acFileName, pLogFileName, MAX_PATH);

	FILE* pLogFile = NULL;
	fopen_s(&pLogFile, m_acFileName, "a+");
	if (NULL == pLogFile)
	{
		printf("打开日志文件失败!!");
		exit(1);
	}
	fseek(pLogFile,0,SEEK_END);
	m_FileSize = ftell(pLogFile);
	fclose(pLogFile);
	
	// 取得初始文件名
	//char acBuffer[MAX_PATH] = {0};
	//
	//for (int i = 0; i < emLevel_Max; i++)
	//{
	//	m_nFileCount[i] = 0;
	//	m_nFileSizeCount[i] = 0;
	//	_snprintf_s(acBuffer, sizeof(acBuffer), "%s_%s_%d.txt", pLogFileName, g_pLogFileName[i], m_nFileCount[i]);
	//	// 打开初始文件
	//	fopen_s(&m_pLogFile[i], acBuffer, "a+");
	//	if (NULL == m_pLogFile[i])
	//	{
	//		printf("打开日志文件失败!!");
	//		exit(1);
	//	}
	//	fseek(m_pLogFile[i],0,SEEK_END);
	//	m_nFileSizeCount[i] = ftell(m_pLogFile[i]);
	//	fseek(m_pLogFile[i], 0, SEEK_SET);
	//}

	m_pInst = this;
}

CLogger::~CLogger(void)
{
	//for (int i = 0; i < emLevel_Max; i++)
	//{
	//	if (NULL == m_pLogFile[i])
	//	{
	//		continue;
	//	}
	//	fseek(m_pLogFile[i], 0, SEEK_END); 
	//	if (ftell(m_pLogFile[i]) == 0)
	//	{
	//		fclose(m_pLogFile[i]);
	//		char acBuffer[MAX_PATH] = {0};
	//		_snprintf_s(acBuffer, sizeof(acBuffer), "%s_%s_%d.txt", m_acFileName, g_pLogFileName[i], m_nFileCount[i]);
	//		remove(acBuffer);
	//	}
	//}
}

void CLogger::SaveLogFFL(int nLogLevel, char* pFile, char* pFunction, int nLine, const char* msg, va_list va)
{
	if (NULL == msg)
	{
		return;
	}

	m_LogLock.Lock();
	char acBuffer[4096] = {0};
	vsnprintf(acBuffer,sizeof(acBuffer) - 1, msg,va);

	time_t timeNow = time(NULL);
	tm* pTm = localtime(&timeNow);
	char acLogBuffer[4096] = {0};
	int nLen = snprintf(acLogBuffer, sizeof(acLogBuffer) - 1, "%d-%d-%d %d:%d:%d\n%s", 
		pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec, acBuffer);


	if (m_bScreen)
	{
		PrintToScreen(acLogBuffer, nLen);
	}
	else
	{
		SaveLogFile(nLogLevel, acLogBuffer, nLen);
	}
	m_LogLock.UnLock();
}

void CLogger::SaveLog(int nLogLevel, const char* msg, va_list va)
{
	if (NULL == msg)
	{
		return;
	}

	m_LogLock.Lock();
	char acBuffer[4096] = {0};
	vsnprintf(acBuffer,sizeof(acBuffer) - 1, msg,va);

	// 加时间戳
	time_t timeNow = time(NULL);
	tm* pTm = localtime(&timeNow);
	

	char acLogBuffer[4096] = {0};
	int nLen = snprintf(acLogBuffer, sizeof(acLogBuffer) - 1, "%d-%d-%d %d:%d:%d\n%s", 
		pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec, acBuffer);


	if (m_bScreen)
	{
		PrintToScreen(acLogBuffer, nLen);
	}
	else
	{
		SaveLogFile(nLogLevel, acLogBuffer, nLen);
	}
	m_LogLock.UnLock();
}

void CLogger::SaveLogFile(int nLogLevel, char *pBuffer, int nLen)
{
	if (NULL == pBuffer)
	{
		return;
	}

	FILE* pLogFile;
	fopen_s(&pLogFile, m_acFileName, "a+");
	if (NULL != pLogFile)
	{
		int nWriteCount = 0;
		while (nWriteCount < nLen)
		{
			int nResult = (unsigned int)fwrite(pBuffer, 1, nLen, pLogFile);
			nWriteCount += nResult;
		}
		fclose(pLogFile);
		m_FileSize += nLen;
		if (m_FileSize > emLogFileMaxSize)
		{
			m_FileSize = 0;
			int nIndex = GetNextFileIndex();
			char acBuffer[MAX_PATH] = "";
			snprintf(acBuffer, sizeof(acBuffer) - 1, "%s.%d", m_acFileName, nIndex);
			rename(m_acFileName, acBuffer);
		}
	}


}

void CLogger::PrintToScreen(char* pBuffer, int nLen)
{
	if (NULL == pBuffer)
	{
		return;
	}
	printf(pBuffer);
}

void CLogger::ClearCache()
{
	m_LogLock.Lock();
	//m_nCacheLen = 0;
	m_LogLock.UnLock();
}

int CLogger::GetNextFileIndex()
{
	int nIndex = 0;
	for (int i = 0; i < emLogFileMax; i++)
	{
		char acBuffer[MAX_PATH] = "";
		FILE* pFile = NULL; 
		snprintf(acBuffer, sizeof(acBuffer) - 1, "%s.%d", m_acFileName, i);
		fopen_s(&pFile, acBuffer, "r");
		if (NULL == pFile)
		{
			nIndex = i;
			char acNextBuffer[MAX_PATH] = "";
			FILE* pNextFile = NULL; 
			snprintf(acNextBuffer, sizeof(acNextBuffer) - 1, "%s.%d", m_acFileName, (i + 1)%emLogFileMax);
			fopen_s(&pNextFile, acNextBuffer, "r");
			if (NULL != pNextFile)
			{
				fclose(pNextFile);
				remove(acNextBuffer);
			}
			break;
		}
		else
		{
			fclose(pFile);
		}
	}
	
	return nIndex;
}


void CLogger::StartNextFile(int nLogLevel)
{
	//if (NULL != m_pLogFile)
	//{
	//	fclose(m_pLogFile[nLogLevel]);
	//}
	//m_nFileSizeCount[nLogLevel] = 0;
	//m_nFileCount[nLogLevel]++;
	//if (m_nFileCount[nLogLevel] >= emLogFileMax)
	//{
	//	m_nFileCount[nLogLevel] = 0;
	//}
	//char acBuffer[MAX_PATH] = {0};
	//_snprintf_s(acBuffer, sizeof(acBuffer), "%s_%s_%d.txt", m_acFileName, g_pLogFileName[nLogLevel], m_nFileCount[nLogLevel]);
	//// 打开初始文件
	//fopen_s(&m_pLogFile[nLogLevel], acBuffer, "w");

}