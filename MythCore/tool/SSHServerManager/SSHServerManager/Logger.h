/********************************************************************
	created:	5/8/2011   15:05
	filename: 	Logger.h
	author:		黄建恒（jeffery）

	purpose:	简单的日志类，主要记录Debug模式下的日志，运行时的逻辑
				日志最后会全部发往日志服务器,为了将日志容量控制在一定
				容量，如果设置单个文件的最大大小，最多可以允许写多少个
				文件。

				心得：看到TL开缓冲区保存日志，再一次性写入磁盘，以为
				可以大幅度提升写入速度,然而测试了一下4M和10M的缓存，
				在debug和release中测试10亿次，每次写入10个字符，发现缓
				存的效率只提升了2%。看到磁盘的缓存机制已经足够好了,测
				试环境为windows,不知道linux的结果会怎么样？
*********************************************************************/
#ifndef _LOGGER_H__
#define _LOGGER_H__
#include "CSLock.h"
#include <stdio.h>

#define LOG_DEBUG_FFL(message, ...) LogDebugFFL(__FILE__, __FUNCTION__, __LINE__, message, __VA_ARGS__) 
#define	LOG_ERROR_FFL(message, ...) LogErrorFFL(__FILE__, __FUNCTION__, __LINE__, message, __VA_ARGS__)


void LogDebugFFL		(char* pFile, char* pFunction, int nLine, const char* vFmt, ... );
void LogInfoFFL			(char* pFile, char* pFunction, int nLine,  const char* vFmt, ... );
void LogNoticeFFL		(char* pFile, char* pFunction, int nLine,  const char* vFmt, ... );
void LogWarnFFL			(char* pFile, char* pFunction, int nLine,  const char* vFmt, ... );
void LogErrorFFL		(char* pFile, char* pFunction, int nLine,  const char* vFmt, ... );
void LogFatalFFL		(char* pFile, char* pFunction, int nLine,  const char* vFmt, ... );

#define LOG_DEBUG(message, ...)		LogDebug(message, __VA_ARGS__) 
#define LOG_INFO(message, ...)		LogInfo(message, __VA_ARGS__)
#define LOG_NOTICE(message, ...)	LogNotice(message, __VA_ARGS__)
#define LOG_WARN(message, ...)		LogWarn(message, __VA_ARGS__)
#define	LOG_ERROR(message, ...)		LogError(message, __VA_ARGS__)
#define LOG_Fatal(message, ...)		LogFatal(message, __VA_ARGS__)

void LogDebug			(const char* vFmt, ... );
void LogInfo			(const char* vFmt, ... );
void LogNotice			(const char* vFmt, ... );
void LogWarn			(const char* vFmt, ... );
void LogError			(const char* vFmt, ... );
void LogFatal			(const char* vFmt, ... );

class CLogger
{
public:
	enum TmLogLevel
	{
		emLevel_Fatal  = 0,
		emLevel_Error  = 1,
		emLevel_Warn   = 2,
		emLevel_Notice = 3,
		emLevel_Info   = 4,
		emLevel_Debug  = 5,
		emLevel_Noice = 6,
		emLevel_Max
	};


	enum
	{
		emLogFileMaxSize	= 10 * 1024 * 1024,				// 日志文件大小,10M,超过10M写下一个文件	
		emLogFileMax		= 100,							// 文件数大小，超过文件就将第一个文件清空再写
	};
public:
	CLogger(char* pLogFileName);
	~CLogger(void);

	void SaveLogFFL(int nLogLevel, char* pFile, char* pFunction, int nLine, const char* msg, va_list va);
	void SaveLog(int nLogLevel, const char* msg, va_list va);
	void SaveLogFile(int nLogLevel, char* pBuffer, int nLen);
	void PrintToScreen(char* pBuffer, int nLen);
	void ClearCache();
	void StartNextFile(int nLogLevel);
	int GetNextFileIndex();

public:
	static CLogger*			Inst(){return m_pInst;}

private:
	// 日志锁
	CCSLock				m_LogLock;
	// 输出到文件还是屏幕
	bool				m_bScreen;
	char				m_acFileName[MAX_PATH];
	int					m_FileSize;


private:
	static CLogger*		m_pInst;
};
#endif