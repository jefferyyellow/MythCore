#ifndef __LOGMANAGER_H__
#define __LOGMANAGER_H__
#include "commontype.h"
#include "singleton.h"

#include "hashmap.h"
//#include <string>
#include <stdarg.h>

#ifdef __DEBUG__
#define MAX_LOG_BUFFER_NUM	10240 
#else
#define MAX_LOG_BUFFER_NUM	1024 
#endif

#	define LOG_INFO(fmt, ... )				CLogManager::Inst()->LogInfoMessage(fmt, ##__VA_ARGS__ )
#	define LOG_WARN(fmt, ... )				CLogManager::Inst()->LogWarnMessage(fmt, ##__VA_ARGS__ )
#	define LOG_ERROR(fmt, ... )				CLogManager::Inst()->LogErrorMessage(fmt, ##__VA_ARGS__ )
#	define LOG_DEBUG(logname, fmt, ... )	CLogManager::Inst()->LogDebugMessage(logname, fmt, ##__VA_ARGS__ )

class CLog;
class CLogManager : public CSingleton<CLogManager>
{
	friend class CSingleton<CLogManager>;
public:
 	//typedef std::map<std::string, CLog*> DebugLogMap;
	typedef Myth::CHashMap<const char* , CLog*, 8, 8, 8> DebugLogMap;
private:
	CLogManager()
	{
		Init();
	}
	~CLogManager()
	{
		Finial();
	}

public:
	int 	Init();
	void	Finial();

public:
	CLog*		GetErrorLog(){return mErrorLog;}
	void		SetErrorLog(CLog* pLog){mErrorLog = pLog;}

	CLog*		GetInfoLog(){return mInfoLog;}
	void		SetInfoLog(CLog* pLog){mInfoLog = pLog;}

	CLog*		GetWarnLog(){return mWarnLog;}
	void		SetWarnLog(CLog* pLog){mWarnLog = pLog;}

public:
	void		LogErrorMessage(const char* pFormat, ...);
	void		LogInfoMessage(const char* pFormat, ...);
	void		LogWarnMessage(const char* pFormat, ...);

public:
	/// get exist debug log by name
	CLog*		GetDebugLog(const char* pName);
	/// add debug log with name
	void		AddDebugLog(CLog* pLog, const char* pName);
	/// log debug message
	void		LogDebugMessage(const char* pLogName, const char* pFormat, ...);

public:
	void		FormatDateString(char* pDateBuffer, int nBuffSize);

	void 		FormatLogMessage(char* pDataBuffer, int nBuffSize, const char* pLogTypeName, const char* pFormat, va_list va);
private:
	/// error log
	CLog*		mErrorLog;
	/// info log
	CLog* 		mInfoLog;
	/// warning log
	CLog*		mWarnLog;
	/// debug log
	DebugLogMap	mDebugLog; 
};
#endif
