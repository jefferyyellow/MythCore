/********************************************************************
	created:	5/8/2011   15:05
	filename: 	Logger.h
	author:		�ƽ��㣨jeffery��

	purpose:	�򵥵���־�࣬��Ҫ��¼Debugģʽ�µ���־������ʱ���߼�
				��־����ȫ��������־������,Ϊ�˽���־����������һ��
				������������õ����ļ�������С������������д���ٸ�
				�ļ���

				�ĵã�����TL��������������־����һ����д����̣���Ϊ
				���Դ��������д���ٶ�,Ȼ��������һ��4M��10M�Ļ��棬
				��debug��release�в���10�ڴΣ�ÿ��д��10���ַ������ֻ�
				���Ч��ֻ������2%���������̵Ļ�������Ѿ��㹻����,��
				�Ի���Ϊwindows,��֪��linux�Ľ������ô����
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
		emLogFileMaxSize	= 10 * 1024 * 1024,				// ��־�ļ���С,10M,����10Mд��һ���ļ�	
		emLogFileMax		= 100,							// �ļ�����С�������ļ��ͽ���һ���ļ������д
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
	// ��־��
	CCSLock				m_LogLock;
	// ������ļ�������Ļ
	bool				m_bScreen;
	char				m_acFileName[MAX_PATH];
	int					m_FileSize;


private:
	static CLogger*		m_pInst;
};
#endif